#include"Camera.h"

Camera::Camera(Input* input)
{
	matView = {};
	position = { 0, 0,-10 };
	target = { 0, 0, 0 };
	upVector = { 0, 1, 0 };
	angle = 0.0f;
	this->input = new Input;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	matView = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&target), XMLoadFloat3(&upVector));

	//if (input->GetKey(DIK_UP) || input->GetKey(DIK_DOWN) || input->GetKey(DIK_RIGHT) || input->GetKey(DIK_LEFT))
	//{
	//	// angleラジアンだけY軸まわりに回転。半径は-100
	//	position.x = -100 * sinf(angle);
	//	position.z = -100 * cosf(angle);
	//	//ビュー変換行列を作り直す	
	//}
}