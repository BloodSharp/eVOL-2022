#include "client.h"

float AngleBetween(Vector& v1, Vector& v2)
{
	Vector v4 = v2.Normalize();
	Vector v3 = v1.Normalize();

	return acos(DotProduct(v3, v4)) * static_cast<float>(180.0 / M_PI);
}

Vector& CrossProduct(Vector& vThis, const Vector& a, const Vector& b)
{
	vThis.x = (a.y * b.z) - (a.z * b.y);
	vThis.y = (a.z * b.x) - (a.x * b.z);
	vThis.z = (a.x * b.y) - (a.y * b.x);

	return vThis;
}

QAngle ToEulerAngles(Vector vThis)
{
	float Pitch, Yaw, Length;

	Length = vThis.Length2D();

	if (Length > 0)
	{
		Pitch = (atan2(-vThis.z, Length) * 180 / M_PI);

		if (Pitch < 0)
		{
			Pitch += 360;
		}

		Yaw = (atan2(vThis.y, vThis.x) * 180 / M_PI);

		if (Yaw < 0)
		{
			Yaw += 360;
		}
	}
	else
	{
		Pitch = (vThis.z > 0.0f) ? 270 : 90;
		Yaw = 0;
	}

	return QAngle(Pitch, Yaw, 0);
}

QAngle ToEulerAngles(Vector vThis, Vector* PseudoUp)
{
	Vector Left;

	float	Length, Yaw, Pitch, Roll;

	CrossProduct(Left, *PseudoUp, vThis);

	Left.Normalize();

	Length = vThis.Length2D();

	if (PseudoUp)
	{
		if (Length > 0.001)
		{
			Pitch = (atan2(-vThis.z, Length) * 180 / M_PI);

			if (Pitch < 0)
			{
				Pitch += 360;
			}

			Yaw = (atan2(vThis.y, vThis.x) * 180 / M_PI);

			if (Yaw < 0)
			{
				Yaw += 360;
			}

			float up_z = (Left[1] * vThis.x) - (Left[0] * vThis.y);

			Roll = (atan2(Left[2], up_z) * 180 / M_PI);

			if (Roll < 0)
			{
				Roll += 360;
			}
		}
		else
		{
			Yaw = (atan2(vThis.y, vThis.x) * 180 / M_PI);

			if (Yaw < 0)
			{
				Yaw += 360;
			}

			Pitch = (atan2(-vThis.z, Length) * 180 / M_PI);

			if (Pitch < 0)
			{
				Pitch += 360;
			}

			Roll = 0;
		}
	}
	else
	{
		if (Length > 0)
		{
			Pitch = (atan2(-vThis.z, Length) * 180 / M_PI);

			if (Pitch < 0)
			{
				Pitch += 360;
			}

			Yaw = (atan2(vThis.y, vThis.x) * 180 / M_PI);

			if (Yaw < 0)
			{
				Yaw += 360;
			}
		}
		else
		{
			Pitch = (vThis.z > 0.0f) ? 270 : 90;
			Yaw = 0;
		}
	}

	return  QAngle(Pitch, Yaw, Roll);
}

void AngleMatrix(QAngle& Rotation, float(*matrix)[3])
{
	float sp, sy, sr, cp, cy, cr, radx, rady, radz;

	radx = Rotation.x * (M_PI * 2 / 360);
	rady = Rotation.y * (M_PI * 2 / 360);
	radz = Rotation.z * (M_PI * 2 / 360);

	sp = sin(radx);
	sy = sin(rady);
	sr = sin(radz);

	cp = cos(radx);
	cy = cos(rady);
	cr = cos(radz);

	matrix[0][0] = cp * cy;
	matrix[0][1] = cp * sy;
	matrix[0][2] = -sp;

	matrix[1][0] = 1 * sr * sp * cy + 1 * cr * -sy;
	matrix[1][1] = 1 * sr * sp * sy + 1 * cr * cy;
	matrix[1][2] = 1 * sr * cp;

	matrix[2][0] = cr * sp * cy + -sr * -sy;
	matrix[2][1] = cr * sp * sy + -sr * cy;
	matrix[2][2] = cr * cp;
}

void VectorRotate(Vector& vThis, Vector& In, QAngle& Rotation)
{
	float matRotate[3][3];

	AngleMatrix(Rotation, matRotate);

	vThis.x = DotProduct(In, matRotate[0]);
	vThis.y = DotProduct(In, matRotate[1]);
	vThis.z = DotProduct(In, matRotate[2]);
}