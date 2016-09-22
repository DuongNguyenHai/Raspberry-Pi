######	Date : 22 Sept 2016
######	Author : Nguyễn Hải Dương
##		Raspberry Pi : Web control

Chương trình điều khiển các thiết bị đơn giản từ xa bằng Raspberry Pi thông qua giao diện điều khiển web.

### Yêu cầu

Cài đặt Apache, PHP và mysql trên Raspberry Pi.

## Sử dụng

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
chương trình hỗ trợ. Hãy thử điều khiển với những thiết bị đó trước khi tạo thiết 
bị của riêng mình.	

Tạo master điều khiển. Truy cập thư mục master và biên dịch master :

	$ gcc -o server-thread server-thread.c -lpthread

Điều khiển bằng trình duyệt :

Mở trình duyêt và điền địa chỉ của server trên thanh địa chỉ, ví dụ : 
192.168.1.23/lazy. Trong đó "192.168.1.23" là địa chỉ ip của raspberry trong mạng,
"lazy" là tên thư mục của chương trình.

Chạy chương trình master trên terminal :

	$ ./server-thread


## Các hàm chức năng

Tạo một thiết bị điều khiển mới :

```objc
InsertObject($conn,object-type,object-name,initial-state,color,amplitude,icon)
```
Trong đó :
-	$conn : là biến liên kết tới mysql
-	object-type : loại thiết bị, loại thiết bị khác nhau có cách điều khiển khác 
nhau. Hỗ trợ 2 lọai là "obj-slider" và "obj-button"
-	object-name : tên thiết bị điều khiển.
-	initial-state : trạng thái thiết lập. 1 tương ứng với bật và 0 tương ứng với
tắt.
-	color : màu sắc cho giao diện điều khiển. Có 4 màu là blue (mặc định : NULL),
green (flavor-green), orange (flavor-orange), violet (flavor-violet).
-	amplitude : Mức hoạt động của thiết bị. Ví dụ như đèn sẽ sáng bao nhiều % so
với định mức.
-	icon : icon đại diện cho thiết bị. Mặc định là "fa-wrench". Xem tên các icon ở đây [font-awsome](http://fontawesome.io/icons/)





