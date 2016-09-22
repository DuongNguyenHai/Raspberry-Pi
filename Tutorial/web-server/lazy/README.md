######	Date : 22 Sept 2016
######	Author : Nguyễn Hải Dương
##		Raspberry Pi : Web control

Chương trình điều khiển các thiết bị đơn giản từ xa bằng Raspberry Pi thông qua giao diện điều khiển web.

### Yêu cầu

Cài đặt Apache, PHP và mysql trên Raspberry Pi.

## Sử dụng

1.	Tải chương trình xuống và bỏ vào thư mục /var/www/html.

2.	Mở tệp sql/sql-function.php lên và sửa 2 dòng sau :
	-	define("userName", "root") thành define("userName", "tên khi bạn cài đặt mysql- mặc định là root").
	-	define("password", "password") thành define("password", "mật khẩu khi cài đặt mysql ứng với tên bên trên").

3.	Tạo database cho các thiết bị 

Gõ lệnh trên terminal : ```sh php sql.php ``` . Chương trình sẽ tạo ra những ví 
dụ về những thiết bị điều khiển đơn giản mà chương trình hỗ trợ. Hãy thử điều khiển
với những thiết bị đó trước khi tạo thiết bị của riêng mình.

4.	Tạo master điều khiển

Truy cập thư mục master và biên dịch master :
```bash
gcc -o server-thread server-thread.c -lpthread
```

4.	Điều khiển bằng trình duyệt.

Mở trình duyêt và điền địa chỉ của server trên thanh địa chỉ, ví dụ : 192.168.1.23/lazy. 
"192.168.1.23" là địa chỉ ip của raspberry trong mạng, "lazy" là tên thư mục của chương trình.

Chạy chương trình master trên terminal :
```bash
./server-thread
```

## Các hàm chức năng






