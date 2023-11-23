//
// Created by cxc on 2023/11/23.
//

#pragma once
#include <array>

class Camera
{
public:

private:
    //以下三个参数共同确定相机的空间位置
    std::array<double, 3> position_;       //当前视点位置(空间坐标)
    std::array<double, 3> focal_position_; //当前焦点位置(空间坐标)
    std::array<double, 3> view_up_;        //相机正方向(归一化XYZ向量)

};
