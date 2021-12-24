#include <map>
#include <vector>
#include <iostream>

#define GRAPHICS_API_OPENGL_43
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <noise/noise.h>

auto default_font = GetFontDefault();
auto default_material = LoadMaterialDefault();

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
	InitWindow(800, 600, "yeah");

	Camera3D camera {};
	camera.target = { 0, 0, 0 };
	camera.position = { 64, 48, 64 };
	camera.up = { 0, 1, 0 };
	camera.projection = CAMERA_PERSPECTIVE;

	float zoom = 1;

	auto shader = LoadShader("res/shader.frag", "res/shader.frag");
	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	auto ambient_loc = GetShaderLocation(shader, "ambient");
	float ambient_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	SetShaderValue(shader, ambient_loc, ambient_color, SHADER_UNIFORM_VEC4);
	default_material.shader = shader;

	Matrix y_rotation = MatrixRotateY(45 * DEG2RAD);
	Matrix rotation = MatrixMultiply(y_rotation, MatrixRotateX(30 * DEG2RAD));

	Vector3 forward_rot = Vector3Transform(Vector3(0, 0, -1), y_rotation);
	std::cout << forward_rot.x << "," << forward_rot.y << "," << forward_rot.z << std::endl;

	auto res = 6;
	auto plane = LoadModelFromMesh(GenMeshPlane(100, 100, res, res));
	plane.materials[0].shader = shader;

	while (!WindowShouldClose())
	{
		auto sc_width = GetScreenWidth(), sc_height = GetScreenHeight();
		auto sc_widthf = float(sc_width), sc_heightf = float(sc_height);
		camera.fovy = 80;

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			auto [mouse_x, mouse_y] = GetMouseDelta();
			auto mouse_delta_world = Vector3Transform(Vector3(-mouse_y, 0, mouse_x), y_rotation);
			camera.position = Vector3Add(camera.position, mouse_delta_world);
			camera.target = Vector3Add(camera.target, mouse_delta_world);
		}

		UpdateCamera(&camera);
		BeginDrawing();
		{
			ClearBackground(BLACK);

			BeginMode3D(camera);
			{
				DrawModel(plane, { 0, 0, 0 }, 1, WHITE);
				//world.Render();
			}
			EndMode3D();
		}
		EndDrawing();
	}
}
