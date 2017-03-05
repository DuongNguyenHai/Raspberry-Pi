######	Date : 12 Jun 2016
######	Author : Nguyễn Hải Dương
##		Raspberry-Pi Tutorial, Project and Libraries

Tổng hợp code trong tutorial về Raspberry Pi. Phần lớn các bài được viết bằng 2 ngôn ngữ C và python.

1.  List bài căn bản bao gồm :
   - GPIO căn bản
   - Giao tiếp I2C
   - Giao tiếp SPI
   - Giao tiếp USB
   - Giao tiếp UART

2.   Các project khác :
   - Project server - client : dựa trên giao thức mạng TCP/IP
   - Project web - server : điều khiển các thiết bị qua web interface
   - Project GPS L70-R : sử dụng module GPS L70-R để tra tọa độ và sử dụng tọa độ để tra địa chỉ bằng google map

3. Xây dựng thư viện cho Pi :

   Mục đích mang đến cách làm việc đơn giản và hiệu quả hơn tới người dùng. Thư viện sẽ ưu tiên việc viết hàm và cách thức làm việc tương đồng với Wiring framework (Arduino cũng theo framework này). Vì thế người dùng có thể sử dụng các thư viện có sẵn từ arduino hay một số dòng vi điều khiển khác sử dụng wiring để dùng với Raspberry Pi. Ngoài ra có xây dựng thêm thư viện cho các module.

   - Thư viện I2C : tương thích với code I2C trên arduino.
   - Thư viện cho module ánh sáng BH1750.
   - Thư viện cho module thời gian thực DSB3231.

Các bài viết hướng dẫn được đăng trong [smart-techvn.com/raspberry](http://smart-techvn.com/bai-viet-ki-thuat/raspberry-pi)
