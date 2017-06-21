drop table if exists account;
create table account(account_id INTEGER PRIMARY KEY, account VARCHAR(255), password TEXT, status_id INTEGER, fullname VARCHAR(255), identity_number CHAR(10), gender_id INTEGER, birthdate DATE, email VARCHAR(255), job VARCHAR(50), avatar BLOB, UNIQUE (account));
insert into account(account, password, status_id, fullname, identity_number, gender_id, birthdate, email, job, avatar) VALUES 
('conbocuoi', '123456', 1, 'Nguyễn Hồ Minh Phước', '01234567', 1, '2017-01-01', 'conbocuoi@hotmail.com', 'Lao động', readfile('media/images/default.png')),
("annguyen97","annguyen97",1,"Nguyễn Văn An","241400001",1,"1997-01-01","annguyen97@gmail.com","Sinh viên",readfile('media/images/default.png')),
("binhle96","binhle96",1,"Lê Bình","241400002",1,"1996-02-01","binhle96@gmail.com","Sinh viên",readfile('media/images/default.png')),
("chipham97","chipham97",1,"Phạm Diệp Chi","241400003",2,"1997-03-05","chipham97@gmail.com","Sinh viên",readfile('media/images/default.png')),
("dungle95","dungle95",1,"Lê Quang Dũng","241400004",1,"1995-03-05","dungle95@gmail.com","nhân viên văn phòng",readfile('media/images/default.png')),
("ennguyen98","ennguyen98",1,"Nguyễn Minh Én","241400005",2,"1998-09-04","ennguyen98@gmail.com","Sinh viên",readfile('media/images/default.png')),
("giangdo93","giangdo93",1,"Đỗ Giang","241400006",1,"1993-04-07","giangdo93@gmail.com","giáo viên",readfile('media/images/default.png')),
("fionafung","fionafung",1,"Fiona Fung","241400007",2,"1994-02-05","fungfi94@gmail.com","nhân viên văn phòng",readfile('media/images/default.png')),
("huongle96","huongle96",1,"Lê Thị Hương","241400008",2,"1996-03-07","huongle96@gmail.com","Sinh viên",readfile('media/images/default.png')),
("iowakabei","iowa00",1,"Iowa Kabeito","241400009",1,"2000-03-02","iowa2000@gmail.com","học sinh",readfile('media/images/default.png')),
("jessi90","jessi90",1,"Jessi J","241400010",2,"1990-02-09","jessij90@gmail.com","giảng viên",readfile('media/images/default.png')),
("khangle22","khangle22",1,"Lê An Khang","241400011",1,"1995-02-11","khangle22@gmail.com","nhân viên văn phòng",readfile('media/images/default.png')),
("lamnguyen97","lamnguyen97",1,"Nguyễn Lâm","241400012",1,"1997-07-05","lamnguyen97@gmail.com","Sinh viên",readfile('media/images/default.png')),
("minhtran93","minhtran93",1,"Trần Công Minh","241400013",1,"1993-03-05","minhtran93@gmail.com","kỹ sư",readfile('media/images/default.png')),
("ngocanh24","ngocanh24",1,"Trần Anh Ngọc","241400014",2,"1996-03-12","ngocanh24@gmai;.com","Sinh viên",readfile('media/images/default.png')),
("oanhpham91","oanhpham91",1,"Phạm Tú Oanh","241400015",2,"1991-04-23","oanhpham91@gmail.com","nội trợ",readfile('media/images/default.png')),
("phuongle94","phuongle94",1,"Lê Thu Phương","241400016",2,"1991-04-21","phuongle94@gmail.com","Sinh viên",readfile('media/images/default.png')),
("quangminh94","quangminh94",1,"Lê Minh Quang","241400017",1,"1994-02-08","quangminh94@gmail.com","Sinh viên",readfile('media/images/default.png')),
("richardle93","richardle20",1,"Richard Lê","241400018",1,"1993-02-03","richardle93@gmail.com","nhân viên văn phòng",readfile('media/images/default.png')),
("sangnguyen97","sangnguyen97",1,"Nguyễn Viết Sang","241400019",1,"1997-03-04","sangnguyen97@gmail.com","nhân viên văn phòng",readfile('media/images/default.png'));

