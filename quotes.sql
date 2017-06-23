DROP TABLE IF EXISTS quotes;
CREATE TABLE quotes(quote VARCHAR(255), author VARCHAR(255));
INSERT INTO quotes VALUES("Một người không đọc sách chẳng hơn gì kẻ không biết đọc.", "Mark Twain"),
("Sách hay, cũng như bạn tốt, ít và được chọn lựa; chọn lựa càng nhiều, thưởng thức càng nhiều.", "Louisa May Alcott"),
("Một cuốn sách hay trên giá sách là một người bạn dù quay lưng lại nhưng vẫn là bạn tốt.", "Khuyết danh"), 
("Đọc sách hay cũng giống như trò truyện với các bộ óc tuyệt vời nhất của những thế kỷ đã trôi qua.", "Rene Descartes"),
("Bạn càng đọc nhiều, bạn càng biết nhiều. Bạn càng học nhiều, bạn càng đi nhiều.", "Victor Hugo"),
("Trong những cuốn sách ẩn chứa linh hồn của suốt chiều dài quá khứ.", "Thomas Carlyle"),
("Không cần phải đốt sách để phá hủy một nền văn hóa. Chỉ cần buộc người ta ngừng đọc mà thôi.", "Mahatma Gandhi"),
("Tôi càng đọc, tôi càng suy ngẫm; và tôi càng được nhiều, tôi càng có thể tin chắc mình không biết điều gì.", "Voltaire"),
("Sách là nguồn của cải quý báu của thế giới và là di sản xứng đáng của các thế hệ và các quốc gia.", "Henry David Thoreau");
SELECT * FROM quotes;