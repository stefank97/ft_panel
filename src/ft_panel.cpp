#include "ft_panel/ft_panel.hpp"
#include <pluginlib/class_list_macros.hpp>
#include <cmath>
#include <rviz_common/display_context.hpp>
#include <QVBoxLayout>
#include <QPushButton>



namespace ft_panel {

    FtPanel::FtPanel(QWidget* parent) : rviz_common::Panel(parent) 
    {
        auto main_layout = new QVBoxLayout(this);
        auto layout_oX = new QHBoxLayout();
        auto layout_oY = new QHBoxLayout();
        auto layout_oYaw = new QHBoxLayout();

        //Sliders:
        oX_slider_ = new QSlider(Qt::Horizontal,this);
        // oX_slider_->setMaximum(1);
        // oX_slider_->setMinimum(-1);
        oX_slider_->setRange(-10,10);
        oX_slider_->setValue(0);
        oX_slider_->setSingleStep(1);
        QObject::connect(oX_slider_, &QSlider::valueChanged, this, &FtPanel::changeDriftX);

        oY_slider_ = new QSlider(Qt::Horizontal,this);
        oY_slider_->setMaximum(10);
        oY_slider_->setMinimum(-10);
        oY_slider_->setValue(0);
        oY_slider_->setSingleStep(1);
        QObject::connect(oY_slider_, &QSlider::valueChanged, this, &FtPanel::changeDriftY);

        oYaw_slider_ = new QSlider(Qt::Horizontal,this);
        oYaw_slider_->setMaximum(180);
        oYaw_slider_->setMinimum(-180);
        oYaw_slider_->setValue(0);
        oYaw_slider_->setSingleStep(1);
        QObject::connect(oYaw_slider_, &QSlider::valueChanged, this, &FtPanel::changeDriftYaw);

        //Label:
        label_oX_ = new QLabel(this);
        label_oX_->setText("Offset Drift x: ");

        layout_oX->addWidget(label_oX_);
        layout_oX->addWidget(oX_slider_);
        main_layout->addLayout(layout_oX);
        
        label_oY_ = new QLabel(this);
        label_oY_->setText("Offset Drift y: ");
        layout_oY->addWidget(label_oY_);
        layout_oY->addWidget(oY_slider_);
        main_layout->addLayout(layout_oY);


        label_oYaw_ = new QLabel(this);
        label_oYaw_->setText("Offset Yaw Degrees: ");
        layout_oYaw->addWidget(label_oYaw_);
        layout_oYaw->addWidget(oYaw_slider_);
        main_layout->addLayout(layout_oYaw);

        //Reset Button
        reset_button_ = new QPushButton("Reset Offsets", this);
        main_layout->addWidget(reset_button_);
        QObject::connect(reset_button_, &QPushButton::clicked, this, &FtPanel::resetOffsets);











    }
    void FtPanel::onInitialize(){
        auto node_ptr_ = getDisplayContext()->getRosNodeAbstraction().lock();
        node_ = node_ptr_->get_raw_node();

        pub_ = node_->create_publisher<geometry_msgs::msg::Vector3>("/fake_transform/drift", rclcpp::QoS(10));
    }

    void FtPanel::changeDriftX(int value){
        oX_ = static_cast<double>(value) * 0.001;
        label_oX_->setText("Offset x: " +QString::number(value));
        publishDrift();
    }
    void FtPanel::changeDriftY(int value){
        oY_ = static_cast<double>(value) * 0.001;
        label_oY_->setText("Offset y: " +QString::number(value));
        publishDrift();
    }
    void FtPanel::changeDriftYaw(int value){
        oYaw_deg_ = static_cast<double>(value) * 0.01;
        label_oYaw_->setText("Offset yaw (deg): " +QString::number(value));
        publishDrift();
    }

    void FtPanel::publishDrift(){

        geometry_msgs::msg::Vector3 msg;
        msg.x = oX_;
        msg.y = oY_;

        double oYaw_rad = oYaw_deg_ * M_PI / 180.0; // Convert to radians
        msg.z = oYaw_rad;
        pub_->publish(msg);
    }

    void FtPanel::resetOffsets() {
        oX_ = 0.0;
        oY_ = 0.0;
        oYaw_deg_ = 0.0;

        oX_slider_->setValue(0);
        oY_slider_->setValue(0);
        oYaw_slider_->setValue(0);

        label_oX_->setText("Offset x: 0");
        label_oY_->setText("Offset y: 0");
        label_oYaw_->setText("Offset yaw (deg): 0");

        publishDrift();
    }



}
PLUGINLIB_EXPORT_CLASS(ft_panel::FtPanel, rviz_common::Panel)











//Display, was wrong does not work like intended.

// #include "ft_display/ft_display.hpp"
// #include <pluginlib/class_list_macros.hpp>
// #include <cmath>
// #include <rviz_common/display_context.hpp>




// namespace ft_display {

//     FtDisplay::FtDisplay()
//     {
//         auto parent = this;
//         offset_drift_x_prop_ = new rviz_common::properties::FloatProperty("Offset Drift X (m)", 0.0, "Set the X offset drift for the fake transform", parent, SLOT(updateOffset()));
//         offset_drift_y_prop_ = new rviz_common::properties::FloatProperty("Offset Drift Y (m)", 0.0, "Set the Y offset drift for the fake transform", parent,SLOT(updateOffset()));
//         offset_drift_yaw_deg_prop_ = new rviz_common::properties::FloatProperty("Offset Drift Yaw (deg)", 0.0, "Set the Yaw offset drift for the fake transform in degree", parent,SLOT(updateOffset()));

//     }

//     void FtDisplay::onInitialize(){
//         auto node_ptr_ = context_->getRosNodeAbstraction().lock();
//         rclcpp::Node::SharedPtr node_ = node_ptr_->get_raw_node();

//         pub_ = node_->create_publisher<geometry_msgs::msg::Vector3>("/fake_transform/drift", rclcpp::QoS(10));
//     }


//     void FtDisplay::updateOffset(){
//         double offset_x = static_cast<double>(offset_drift_x_prop_->getFloat());
//         double offset_y = static_cast<double>(offset_drift_y_prop_->getFloat());
//         double offset_yaw_deg = static_cast<double>(offset_drift_yaw_deg_prop_->getFloat());
//         double offset_yaw = offset_yaw_deg * M_PI / 180.0; // Convert to radians

//         geometry_msgs::msg::Vector3 offset_msg;
//         offset_msg.x = offset_x;
//         offset_msg.y = offset_y;
//         offset_msg.z = offset_yaw;

//         RCLCPP_DEBUG(
//             rclcpp::get_logger("rviz"),
//             "Publishing offset drift: x=%f, y=%f, yaw=%f radians",
//             offset_x, offset_y, offset_yaw
//         );

//         pub_->publish(offset_msg);
//     }
// }
// PLUGINLIB_EXPORT_CLASS(ft_display::FtDisplay, rviz_common::Display)