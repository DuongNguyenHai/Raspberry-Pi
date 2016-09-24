######	Date : 22 Sept 2016
######	Author : Nguyễn Hải Dương
##		Raspberry Pi : Web control

Chương trình điều khiển các thiết bị đơn giản từ xa bằng Raspberry Pi thông qua 
giao diện điều khiển web.

### Yêu cầu

Cài đặt Apache, PHP và mysql trên Raspberry Pi. Có thể xem hướng dẫn [ở đây]
(http://smart-techvn.com/bai-viet-ki-thuat/raspberry-pi/24025-bai-10-lap-trinh-web-server-tren-raspberry-pi-%E2%80%93-phan-1.html)

### Tính năng

Mô hình có dạng tương tác như sau :

Web <--> Server <--> Master <--> Device

Người dùng sử dụng web từ máy tính hoặc điện thoại kết nối tới server trên
Raspberry Pi. Sử dụng giao diên web để gửi thông tin điều khiển tới server. Server
sử dụng mysql làm cơ sở dữ liệu để lưu trữ thông tin của các thiết bị. Những thông
tin này sẽ được cập nhật tương ứng với điều khiển của người dùng. Ngay sau khi 
nhận thông tin từ web, Server gửi thông tin đó tới Master. Master là trung tâm 
điều khiển viết bằng ngôn ngữ C. Thực ra Master giống như cây cầu kết nối giữa 
Server và các thiết bị. Nó nhận thông tin và gửi tín hiệu điều khiển tới thiết 
bị tương ứng. Vì viết bằng ngôn ngữ C nên Master có thể sử dụng rộng rãi nhiều 
giao thức hơn (không chỉ là internet) trong việc kết nối với thiết bị như Zigbee, 
RF ... . Chương trình chỉ thực hiện tới phần Master mà không thực hiện việc giao
tiếp giữa Master và thiết bị. Master chỉ hiện thị thông tin nhận được từ Server và
người dùng sẽ phải tự quyết định làm gì với dữ liệu đó.

Khi cài đặt xong chương trình sẽ có sẵn giao diện điều khiển cho một vài thiêt
bị đơn giản như đèn điên, cửa ra vào. 

### Cài đặt và sử dụng

Tải chương trình xuống và bỏ vào thư mục /var/www/html. Có thể sử dụng git command

	$ cd /var/www/html
	$ git clone https://github.com/season93vn/Raspberry-Pi.git

Mở tệp sql/sql-function.php lên và sửa 2 dòng sau :

-	define("userName", "root") thành define("userName", "tên khi bạn cài đặt
mysql, mặc định là root").

-	define("password", "password") thành define("password", "mật khẩu khi
cài đặt mysql ứng với tên bên trên").

Tạo database cho các thiết bị. Gõ lệnh trên terminal : 

	$ php sql.php

Chương trình sẽ tạo ra những ví dụ về những thiết bị điều khiển đơn giản mà 
chương trình hỗ trợ. Hãy thử điều khiển với những thiết bị đó trước khi tạo 
thiết bị của riêng mình.	

Tạo master điều khiển. Truy cập thư mục master và biên dịch master :

	$ gcc -o server-thread server-thread.c -lpthread

Điều khiển bằng trình duyệt :

Mở trình duyêt và điền địa chỉ của server trên thanh địa chỉ, ví dụ : 
192.168.1.23/lazy. Trong đó "192.168.1.23" là địa chỉ ip của raspberry trong mạng,
"lazy" là tên thư mục của chương trình.

Chạy chương trình master trên terminal :

	$ ./server-thread

### Các hàm chức năng

Cách hàm chức năng viết trong file "sql-function.php" và được sử dụng trong file
"sql.php". Mục đích để xử lý cơ sở dữ liệu trên mysql. Bao gồm thêm vào, xóa, cập nhật
thiêt bị vào cơ sở dữ liệu.

Tạo một thiết bị điều khiển mới :

```objc

InsertObject($conn,object-type,object-name,initial-state,color,amplitude,icon)

```
Trong đó :

-	$conn : là biến liên kết tới mysql
-	object-type (string) : loại thiết bị, loại thiết bị khác nhau có cách điều khiển khác 
nhau. Hỗ trợ 2 lọai là "obj-slider" và "obj-button"
-	object-name (string) : tên thiết bị điều khiển.
-	initial-state : trạng thái thiết lập. 1 tương ứng với bật và 0 tương ứng với
	tắt.
-	color (string) : màu sắc cho giao diện điều khiển. Có 4 màu là blue (mặc định : NULL),
	green (flavor-green), orange (flavor-orange), violet (flavor-violet).
-	amplitude (number) : Mức hoạt động của thiết bị. Ví dụ như đèn sẽ sáng bao nhiều % so
	với định mức.
-	icon (string) : icon đại diện cho thiết bị. Mặc định là "fa-wrench". Xem tên các icon 
	ở đây [Font Awesome](http://fontawesome.io/icons/)

Xoá một thiết bị điều khiển :

```objc

DeleteObject($conn, object-name)

```
Trong đó :

-	object-name (string) : tên thiết bị muốn xóa

Update thông tin cho thiết bị

```objc

UpdateObject($conn, object-name,"name='value'")

```

Trong đó :

-	object-name (string) : tên thiết bị muốn update
-	name (string) : tên thuộc tính muốn thay đổi
-	value : giá trị muốn thay đổi.

Để thực thi file "sql.php". Gõ lệnh :

	$ php sql.php

Có thể xem bài hướng dẫn chi tiết ở [web-server]()
