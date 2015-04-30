/*
* Implementation file for the contact resolution system.
*
* Part of the Cyclone physics system.
*
* Copyright (c) Icosagon 2003. All Rights Reserved.
*
* This software is distributed under licence. Use of this software
* implies agreement with all terms and conditions of the accompanying
* software licence.
*/

#include "contacts.h"
#include <memory.h>
#include <assert.h>

using namespace cyclone;

// Contact implementation

void Contact::setBodyData(RigidBody* one, RigidBody *two,
	float friction, float restitution)
{
	Contact::body[0] = one;
	Contact::body[1] = two;
	Contact::friction = friction;
	Contact::restitution = restitution;
}

void Contact::matchAwakeState()
{
	// Collisions with the world never cause a body to wake up.
	if (!body[1]) return;

	bool body0awake = body[0]->GetAwake();
	bool body1awake = body[1]->GetAwake();

	// Wake up only the sleeping one
	if (body0awake ^ body1awake) {
		if (body0awake) body[1]->SetAwake();
		else body[0]->SetAwake();
	}
}

/*
* Swaps the bodies in the current contact, so body 0 is at body 1 and
* vice versa. This also changes the direction of the contact normal,
* but doesn't update any calculated internal data. If you are calling
* this method manually, then call calculateInternals afterwards to
* make sure the internal data is up to date.
*/
void Contact::swapBodies()
{
	contactNormal *= -1;

	RigidBody *temp = body[0];
	body[0] = body[1];
	body[1] = temp;
}

/*
* Constructs an arbitrary orthonormal basis for the contact.  This is
* stored as a 3x3 matrix, where each vector is a column (in other
* words the matrix transforms contact space into world space). The x
* direction is generated from the contact normal, and the y and z
* directionss are set so they are at right angles to it.
*/
inline
void Contact::calculateContactBasis()
{
	Vector3D contactTangent[2];

	// Check whether the Z-axis is nearer to the X or Y axis
	if (abs(contactNormal.X) > abs(contactNormal.Y))
	{
		// Scaling factor to ensure the results are normalised
		const float s = (float)1.0f / std::sqrt(contactNormal.Z*contactNormal.Z +
			contactNormal.X*contactNormal.X);

		// The new X-axis is at right angles to the world Y-axis
		contactTangent[0].X = contactNormal.Z*s;
		contactTangent[0].Y = 0;
		contactTangent[0].Z = -contactNormal.X*s;

		// The new Y-axis is at right angles to the new X- and Z- axes
		contactTangent[1].X = contactNormal.Y*contactTangent[0].X;
		contactTangent[1].Y = contactNormal.Z*contactTangent[0].X -
			contactNormal.X*contactTangent[0].Z;
		contactTangent[1].Z = -contactNormal.Y*contactTangent[0].X;
	}
	else
	{
		// Scaling factor to ensure the results are normalised
		const float s = (float)1.0 / std::sqrt(contactNormal.Z*contactNormal.Z +
			contactNormal.Y*contactNormal.Y);

		// The new X-axis is at right angles to the world X-axis
		contactTangent[0].X = 0;
		contactTangent[0].Y = -contactNormal.Z*s;
		contactTangent[0].Z = contactNormal.Y*s;

		// The new Y-axis is at right angles to the new X- and Z- axes
		contactTangent[1].X = contactNormal.Y*contactTangent[0].Z -
			contactNormal.Z*contactTangent[0].Y;
		contactTangent[1].Y = -contactNormal.X*contactTangent[0].Z;
		contactTangent[1].Z = contactNormal.X*contactTangent[0].Y;
	}

	// Make a matrix from the three vectors.
	contactToWorld.Set(
		contactNormal,
		contactTangent[0],
		contactTangent[1]);
}

Vector3D Contact::calculateLocalVelocity(unsigned bodyIndex, float duration)
{
	RigidBody *thisBody = body[bodyIndex];

	// Work out the velocity of the contact point.
	Vector3D velocity =
		Vector3D (thisBody->getRotation()) % relativeContactPosition[bodyIndex];
	velocity += thisBody->getVelocity();

	// Turn the velocity into contact-coordinates.
	Vector3D contactVelocity = contactToWorld.Transpose().Transform(velocity);

	// Calculate the ammount of velocity that is due to forces without
	// reactions.
	Vector3D accVelocity = thisBody->GetLastFrameAcceleration() * duration;

	// Calculate the velocity in contact-coordinates.
	accVelocity = contactToWorld.Transpose().Transform(accVelocity);

	// We ignore any component of acceleration in the contact normal
	// direction, we are only interested in planar acceleration
	accVelocity.X = 0;

	// Add the planar velocities - if there's enough friction they will
	// be removed during velocity resolution
	contactVelocity += accVelocity;

	// And return it
	return contactVelocity;
}


void Contact::calculateDesiredDeltaVelocity(float duration)
{
	const static float velocityLimit = (float)0.25f;

	// Calculate the acceleration induced velocity accumulated this frame
	float velocityFromAcc = 0;

	if (body[0]->GetAwake())
	{
		velocityFromAcc +=
			body[0]->GetLastFrameAcceleration() * duration * contactNormal;
	}

	if (body[1] && body[1]->GetAwake())
	{
		velocityFromAcc -=
			body[1]->GetLastFrameAcceleration() * duration * contactNormal;
	}

	// If the velocity is very slow, limit the restitution
	float thisRestitution = restitution;
	if (abs(contactVelocity.X) < velocityLimit)
	{
		thisRestitution = (float)0.0f;
	}

	// Combine the bounce velocity with the removed
	// acceleration velocity.
	desiredDeltaVelocity =
		-contactVelocity.X
		- thisRestitution * (contactVelocity.X - velocityFromAcc);
}


void Contact::calculateInternals(float duration)
{
	// Check if the first object is NULL, and swap if it is.
	if (!body[0]) swapBodies();
	assert(body[0]);

	// Calculate an set of axis at the contact point.
	calculateContactBasis();

	// Store the relative position of the contact relative to each body
	relativeContactPosition[0] = contactPoint - body[0]->GetPosition();
	if (body[1]) {
		relativeContactPosition[1] = contactPoint - body[1]->GetPosition();
	}

	// Find the relative velocity of the bodies at the contact point.
	contactVelocity = calculateLocalVelocity(0, duration);
	if (body[1]) {
		contactVelocity -= calculateLocalVelocity(1, duration);
	}

	// Calculate the desired change in velocity for resolution
	calculateDesiredDeltaVelocity(duration);
}

void Contact::applyVelocityChange(Vector3D velocityChange[2],
	Vector3D rotationChange[2])
{
	// Get hold of the inverse mass and inverse inertia tensor, both in
	// world coordinates.
	Matrix inverseInertiaTensor[2];
	body[0]->GetInverseInertiaTensorWorld(inverseInertiaTensor[0]);
	if (body[1])
		body[1]->GetInverseInertiaTensorWorld(inverseInertiaTensor[1]);

	// We will calculate the impulse for each contact axis
	Vector3D impulseContact;

	impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);

	

	// Convert impulse to world coordinates
	Vector3D impulse = contactToWorld.Transform(impulseContact);

	// Split in the impulse into linear and rotational components
	Vector3D impulsiveTorque = relativeContactPosition[0] % impulse;
	rotationChange[0] = inverseInertiaTensor[0].Transform(impulsiveTorque);
	velocityChange[0].clear();
	velocityChange[0] += (impulse * (float)body[0]->getInverseMass());

	// Apply the changes
	body[0]->AddVelocity(velocityChange[0]);
	body[0]->AddRotation(rotationChange[0]);

	if (body[1])
	{
		// Work out body one's linear and angular changes
		Vector3D impulsiveTorque = impulse % relativeContactPosition[1];
		rotationChange[1] = inverseInertiaTensor[1].Transform(impulsiveTorque);
		velocityChange[1].clear();
		velocityChange[1] += (impulse * (float)-body[1]->getInverseMass());

		// And apply them.
		body[1]->AddVelocity(velocityChange[1]);
		body[1]->AddRotation(rotationChange[1]);
	}
}

inline
Vector3D Contact::calculateFrictionlessImpulse(Matrix * inverseInertiaTensor)
{
	Vector3D impulseContact;

	// Build a vector that shows the change in velocity in
	// world space for a unit impulse in the direction of the contact
	// normal.
	Vector3D deltaVelWorld = relativeContactPosition[0] % contactNormal;
	deltaVelWorld = inverseInertiaTensor[0].Transform(deltaVelWorld);
	deltaVelWorld = deltaVelWorld % relativeContactPosition[0];

	// Work out the change in velocity in contact coordiantes.
	float deltaVelocity = deltaVelWorld * contactNormal;

	// Add the linear component of velocity change
	deltaVelocity += (float)body[0]->getInverseMass();

	// Check if we need to the second body's data
	if (body[1])
	{
		// Go through the same transformation sequence again
		Vector3D deltaVelWorld = relativeContactPosition[1] % contactNormal;
		deltaVelWorld = inverseInertiaTensor[1].Transform(deltaVelWorld);
		deltaVelWorld = deltaVelWorld % relativeContactPosition[1];

		// Add the change in velocity due to rotation
		deltaVelocity += deltaVelWorld * contactNormal;

		// Add the change in velocity due to linear motion
		deltaVelocity += (float)body[1]->getInverseMass();
	}

	// Calculate the required size of the impulse
	impulseContact.X = desiredDeltaVelocity / deltaVelocity;
	impulseContact.Y = 0;
	impulseContact.Z = 0;
	return impulseContact;
}
/*
inline
Vector3D Contact::calculateFrictionImpulse(Matrix * inverseInertiaTensor)
{
	Vector3D impulseContact;
	float inverseMass = body[0]->getInverseMass();

	// The equivalent of a cross product in matrices is multiplication
	// by a skew symmetric matrix - we build the matrix for converting
	// between linear and angular quantities.
	Matrix impulseToTorque;
	impulseToTorque.setSkewSymmetric(relativeContactPosition[0]);

	// Build the matrix to convert contact impulse to change in velocity
	// in world coordinates.
	Matrix deltaVelWorld = impulseToTorque;
	deltaVelWorld *= inverseInertiaTensor[0];
	deltaVelWorld *= impulseToTorque;
	deltaVelWorld *= -1;

	// Check if we need to add body two's data
	if (body[1])
	{
		// Set the cross product matrix
		impulseToTorque.setSkewSymmetric(relativeContactPosition[1]);

		// Calculate the velocity change matrix
		Matrix3 deltaVelWorld2 = impulseToTorque;
		deltaVelWorld2 *= inverseInertiaTensor[1];
		deltaVelWorld2 *= impulseToTorque;
		deltaVelWorld2 *= -1;

		// Add to the total delta velocity.
		deltaVelWorld += deltaVelWorld2;

		// Add to the inverse mass
		inverseMass += body[1]->getInverseMass();
	}

	// Do a change of basis to convert into contact coordinates.
	Matrix3 deltaVelocity = contactToWorld.transpose();
	deltaVelocity *= deltaVelWorld;
	deltaVelocity *= contactToWorld;

	// Add in the linear velocity change
	deltaVelocity.data[0] += inverseMass;
	deltaVelocity.data[4] += inverseMass;
	deltaVelocity.data[8] += inverseMass;

	// Invert to get the impulse needed per unit velocity
	Matrix3 impulseMatrix = deltaVelocity.inverse();

	// Find the target velocities to kill
	Vector3D velKill(desiredDeltaVelocity,
		-contactVelocity.Y,
		-contactVelocity.Z);

	// Find the impulse to kill target velocities
	impulseContact = impulseMatrix.transform(velKill);

	// Check for exceeding friction
	float planarImpulse = std::sqrt(
		impulseContact.Y*impulseContact.Y +
		impulseContact.Z*impulseContact.Z
		);
	if (planarImpulse > impulseContact.X * friction)
	{
		// We need to use dynamic friction
		impulseContact.Y /= planarImpulse;
		impulseContact.Z /= planarImpulse;

		impulseContact.X = deltaVelocity.data[0] +
			deltaVelocity.data[1] * friction*impulseContact.Y +
			deltaVelocity.data[2] * friction*impulseContact.Z;
		impulseContact.X = desiredDeltaVelocity / impulseContact.X;
		impulseContact.Y *= friction * impulseContact.X;
		impulseContact.Z *= friction * impulseContact.X;
	}
	return impulseContact;
}*/

void Contact::applyPositionChange(Vector3D linearChange[2],
	Vector3D angularChange[2],
	float penetration)
{
	const float angularLimit = (float)0.2f;
	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	// We need to work out the inertia of each object in the direction
	// of the contact normal, due to angular inertia only.
	for (unsigned i = 0; i < 2; i++) if (body[i])
	{
		Matrix inverseInertiaTensor;
		body[i]->GetInverseInertiaTensorWorld(inverseInertiaTensor);

		// Use the same procedure as for calculating frictionless
		// velocity change to work out the angular inertia.
		Vector3D angularInertiaWorld =
			relativeContactPosition[i] % contactNormal;
		angularInertiaWorld =
			inverseInertiaTensor.Transform(angularInertiaWorld);
		angularInertiaWorld =
			angularInertiaWorld % relativeContactPosition[i];
		angularInertia[i] =
			angularInertiaWorld * contactNormal;

		// The linear component is simply the inverse mass
		linearInertia[i] = (float)body[i]->getInverseMass();

		// Keep track of the total inertia from all components
		totalInertia += linearInertia[i] + angularInertia[i];

		// We break the loop here so that the totalInertia value is
		// completely calculated (by both iterations) before
		// continuing.
	}

	// Loop through again calculating and applying the changes
	for (unsigned i = 0; i < 2; i++) if (body[i])
	{
		// The linear and angular movements required are in proportion to
		// the two inverse inertias.
		float sign = (i == 0) ? 1.0f : -1.0f;
		angularMove[i] =
			sign * penetration * (angularInertia[i] / totalInertia);
		linearMove[i] =
			sign * penetration * (linearInertia[i] / totalInertia);

		// To avoid angular projections that are too great (when mass is large
		// but inertia tensor is small) limit the angular move.
		Vector3D projection = relativeContactPosition[i];
		projection += (contactNormal * ((relativeContactPosition[i] * -1.0f) * contactNormal));

		// Use the small angle approximation for the sine of the angle (i.e.
		// the magnitude would be sine(angularLimit) * projection.magnitude
		// but we approximate sine(angularLimit) to angularLimit).
		float maxMagnitude = angularLimit * projection.magnitude();

		if (angularMove[i] < -maxMagnitude)
		{
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = -maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}
		else if (angularMove[i] > maxMagnitude)
		{
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}

		// We have the linear amount of movement required by turning
		// the rigid body (in angularMove[i]). We now need to
		// calculate the desired rotation to achieve that.
		if (angularMove[i] == 0)
		{
			// Easy case - no angular movement means no rotation.
			angularChange[i].clear();
		}
		else
		{
			// Work out the direction we'd like to rotate in.
			Vector3D targetAngularDirection =
				relativeContactPosition[i].VectorMult(contactNormal);

			Matrix inverseInertiaTensor;
			body[i]->GetInverseInertiaTensorWorld(inverseInertiaTensor);

			// Work out the direction we'd need to rotate to achieve that
			angularChange[i] =
				inverseInertiaTensor.Transform(targetAngularDirection) *
				(angularMove[i] / angularInertia[i]);
		}

		// Velocity change is easier - it is just the linear movement
		// along the contact normal.
		linearChange[i] = contactNormal * linearMove[i];

		// Now we can start to apply the values we've calculated.
		// Apply the linear movement
		Vector3D pos;
		pos = body[i]->GetPosition();
		pos += (contactNormal * linearMove[i]);
		body[i]->setPos(pos);

		// And the change in orientation
		Quaternion q;
		q = body[i]->GetOrientation();
		q.AddScaledVector(angularChange[i], ((float)1.0));
		body[i]->SetOrientation(q);

		// We need to calculate the derived data for any body that is
		// asleep, so that the changes are reflected in the object's
		// data. Otherwise the resolution will not change the position
		// of the object, and the next collision detection round will
		// have the same penetration.
		if (!body[i]->GetAwake()) body[i]->CalculateDerivedData();
	}
}





// Contact resolver implementation

ContactResolver::ContactResolver(unsigned iterations,
	float velocityEpsilon,
	float positionEpsilon)
{
	setIterations(iterations, iterations);
	setEpsilon(velocityEpsilon, positionEpsilon);
}

ContactResolver::ContactResolver(unsigned velocityIterations,
	unsigned positionIterations,
	float velocityEpsilon,
	float positionEpsilon)
{
	setIterations(velocityIterations);
	setEpsilon(velocityEpsilon, positionEpsilon);
}

void ContactResolver::setIterations(unsigned iterations)
{
	setIterations(iterations, iterations);
}

void ContactResolver::setIterations(unsigned velocityIterations,
	unsigned positionIterations)
{
	ContactResolver::velocityIterations = velocityIterations;
	ContactResolver::positionIterations = positionIterations;
}

void ContactResolver::setEpsilon(float velocityEpsilon,
	float positionEpsilon)
{
	ContactResolver::velocityEpsilon = velocityEpsilon;
	ContactResolver::positionEpsilon = positionEpsilon;
}

void ContactResolver::resolveContacts(Contact *contacts,
	unsigned numContacts,
	float duration)
{
	// Make sure we have something to do.
	if (numContacts == 0) return;
	if (!isValid()) return;

	// Prepare the contacts for processing
	prepareContacts(contacts, numContacts, duration);

	// Resolve the interpenetration problems with the contacts.
	adjustPositions(contacts, numContacts, duration);

	// Resolve the velocity problems with the contacts.
	adjustVelocities(contacts, numContacts, duration);
}

void ContactResolver::prepareContacts(Contact* contacts,
	unsigned numContacts,
	float duration)
{
	// Generate contact velocity and axis information.
	Contact* lastContact = contacts + numContacts;
	for (Contact* contact = contacts; contact < lastContact; contact++)
	{
		// Calculate the internal contact data (inertia, basis, etc).
		contact->calculateInternals(duration);
	}
}

void ContactResolver::adjustVelocities(Contact *c,
	unsigned numContacts,
	float duration)
{
	Vector3D velocityChange[2], rotationChange[2];
	Vector3D deltaVel;

	// iteratively handle impacts in order of severity.
	velocityIterationsUsed = 0;
	while (velocityIterationsUsed < velocityIterations)
	{
		// Find contact with maximum magnitude of probable velocity change.
		float max = velocityEpsilon;
		unsigned index = numContacts;
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (c[i].desiredDeltaVelocity > max)
			{
				max = c[i].desiredDeltaVelocity;
				index = i;
			}
		}
		if (index == numContacts) break;

		// Match the awake state at the contact
		c[index].matchAwakeState();

		// Do the resolution on the contact that came out top.
		c[index].applyVelocityChange(velocityChange, rotationChange);

		// With the change in velocity of the two bodies, the update of
		// contact velocities means that some of the relative closing
		// velocities need recomputing.
		for (unsigned i = 0; i < numContacts; i++)
		{
			// Check each body in the contact
			for (unsigned b = 0; b < 2; b++) if (c[i].body[b])
			{
				// Check for a match with each body in the newly
				// resolved contact
				for (unsigned d = 0; d < 2; d++)
				{
					if (c[i].body[b] == c[index].body[d])
					{
						deltaVel = velocityChange[d] + rotationChange[d].VectorMult( c[i].relativeContactPosition[b]);

						// The sign of the change is negative if we're dealing
						// with the second body in a contact.
						c[i].contactVelocity +=
							c[i].contactToWorld.Transpose().Transform(deltaVel)
							* (b ? -1.0f : 1.0f);
						c[i].calculateDesiredDeltaVelocity(duration);
					}
				}
			}
		}
		velocityIterationsUsed++;
	}
}

void ContactResolver::adjustPositions(Contact *c,
	unsigned numContacts,
	float duration)
{
	unsigned i, index;
	Vector3D linearChange[2], angularChange[2];
	float max;
	Vector3D deltaPosition;

	// iteratively resolve interpenetrations in order of severity.
	positionIterationsUsed = 0;
	while (positionIterationsUsed < positionIterations)
	{
		// Find biggest penetration
		max = positionEpsilon;
		index = numContacts;
		for (i = 0; i<numContacts; i++)
		{
			if (c[i].penetration > max)
			{
				max = c[i].penetration;
				index = i;
			}
		}
		if (index == numContacts) break;

		// Match the awake state at the contact
		c[index].matchAwakeState();

		// Resolve the penetration.
		c[index].applyPositionChange(
			linearChange,
			angularChange,
			max);

		// Again this action may have changed the penetration of other
		// bodies, so we update contacts.
		for (i = 0; i < numContacts; i++)
		{
			// Check each body in the contact
			for (unsigned b = 0; b < 2; b++) if (c[i].body[b])
			{
				// Check for a match with each body in the newly
				// resolved contact
				for (unsigned d = 0; d < 2; d++)
				{
					if (c[i].body[b] == c[index].body[d])
					{
						deltaPosition = linearChange[d] +
							angularChange[d].VectorMult(
							c[i].relativeContactPosition[b]);

						// The sign of the change is positive if we're
						// dealing with the second body in a contact
						// and negative otherwise (because we're
						// subtracting the resolution)..
						c[i].penetration +=
							deltaPosition * (c[i].contactNormal)
							* (b ? 1 : -1);
					}
				}
			}
		}
		positionIterationsUsed++;
	}
}