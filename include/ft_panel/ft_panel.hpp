#include <rviz_common/panel.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rviz_common/properties/float_property.hpp>
#include <std_msgs/msg/string.hpp>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

namespace ft_panel {



class FtPanel : public rviz_common::Panel
{
    Q_OBJECT
public:
    explicit FtPanel(QWidget* parent = 0);
 

  void onInitialize() override;


protected Q_SLOTS:
    void changeDriftX(int value);
    void changeDriftY(int value);
    void changeDriftYaw(int value);
    void resetOffsets();
    
private:
    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr pub_;

    QPushButton* reset_button_;

    QLabel* label_oX_;
    QLabel* label_oY_;
    QLabel* label_oYaw_;

    QSlider* oX_slider_;
    QSlider* oY_slider_;
    QSlider* oYaw_slider_;

    double oX_ {0.0};
    double oY_ {0.0};
    double oYaw_deg_ {0.0};

    void publishDrift();
};

}
