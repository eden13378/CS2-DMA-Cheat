#pragma once
#define _USE_MATH_DEFINES
#include "../Game.h"
#include "../OS-ImGui/OS-ImGui.h"
#include <math.h>
#include <tuple>


class Base_Radar
{
public:
	// 设置雷达数据
	void SetSize(const float& Size);
	void SetPos(const Vec2& Pos);
	void SetRange(const float& Range);
	void SetCrossColor(const ImColor& Color);
	void SetProportion(const float& Proportion);
	void SetDrawList(ImDrawList* DrawList);


	std::pair<int, int> rotate_point(const std::pair<int, int>& center, const std::pair<int, int>& point, double angle);
	std::pair<int, int> world_to_minimap2(double x, double y, double pos_x, double pos_y, double scale, int map_w, int map_h);

	// 获取雷达数据
	float GetSize();
	Vec2 GetPos();
	// 添加绘制点
	void AddPoint(const Vec3& LocalPos, const float& LocalYaw, const Vec3& Pos, ImColor Color, float Yaw = 0.0f, int same_team = 1, float hp = 100, int is_bomb = 0);
	// 渲染
	void Save();

	void UpdateMap(std::string mapname);
public:
	ImDrawList* DrawList = nullptr;
	// 十字显示
	bool ShowCrossLine = true;
	// 十字颜色
	ImColor CrossColor = ImColor(255, 255, 255, 255);
	// 比例
	float Proportion = 2680;
	// 圆点半径
	float CircleSize = 4;
	// 箭头尺寸
	float ArrowSize = 11;
	// 圆弧箭头尺寸
	float ArcArrowSize = 7;
	// 雷达范围
	float RenderRange = 250;
	// 本地Yaw数据
	float LocalYaw = 0.0f;
	// 状态
	bool Opened = true;
	// 雷达绘制类型 0:圆形 1:箭头 2:圆弧箭头
	int  PointType = 0;

	void Render();

private:

	Vec2  Pos{ 0,0 };

	float Width = 200;

	std::vector<std::pair<Vec2, std::vector<std::pair<int, float>>>> Points_new;

};