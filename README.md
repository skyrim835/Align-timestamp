# Align-timestamp
对于小觅和realsense imu捷联在相机内部
外部的lidar可以线与camera联合标定，再计算到imu的外参
实际录数据的时候发现lidar时间戳不太对，于是用camera的时间来对齐，找到camera发布的第一个信息，用其时间戳的整秒部分代替lidar的整秒部分，保留lidar
的纳秒部分，再利用前后lida旧时间戳的间隔递推后续lidar时间戳
（需要自己创建ros包）
