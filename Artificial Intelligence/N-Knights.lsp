;;;; N-Knights problem

;;; Program that can determine how many knights can be placed on a chessboard so that no two knights attack each other. A knight can attack another knight if their horizontal distance is 1 and their vertical distance is 2 or vice versa (i.e. if they are L- shaped apart). Now, assume that some of the squares of the chessboard have been marked as unusable; therefore, no knight can be placed on them.

;;; Input format
;;; A list containing the size of the board and the coordinates of the unusable locations:
;;; (place-knights l)
;;; where l is a list in the following format:
;;; ((n m) ((x1 y1) (x2 y2) ... (xn ym)))
;;; n is the total number of rows and m the total number of columns of the board, and 0<M,N≤30.
;;; x and y are the coordinates on which knights cannot be placed on.

;;; Output format
;;; ((w1 z1) ... (wn zm))
;;; w and z are the coordinates on which knights can be placed on.
	
	
	
	
 (defun reca (i list1 m n)  									;Form the grid locations 
	 (if (< i n) 
	 	(progn (setf j 1) 
			(defun rec_add ( j k list1 n)
     			(if (< j (+ m 1)) (setf list1 (append (list (list (+ 1 (- n k)) j)) (rec_add (+ j 1) k list1 n))) 
					nil)) 
					(setf list1 (append (reca (+ i 1) list1 m n) (rec_add j (+ i 1) list1 n))))))


 (defun i_not_in (list d_use1)									;Check for element in list
        (if (member list d_use1 :test #'equal) T))

(defun place-knights (list)										;Call this function to run the program
  (setf fri (pop list))
  (setf return_exist '())
  (setf grid (reca 0 '() (cadr fri) (car fri)) )
  (setf return_exist (check grid list (car fri) (cadr fri))))


(defun check (grid list m n)									;Applies rules
  (if (endp grid) ()    
     (progn
       (setf next (pop grid))
       (if (i_not_in next list) (check grid list m n)
         (progn
           (setf i (first next))
           (Setf j (cadr next)) 
			   (cond 
				   ((and (> (+ i 1) 0) (<= (+ i 1) m) (> (+ j 2) 0) (<= (+ j 2) n)) (setf list (append list (list (list (+ i 1) (+ j 2))))))
				   ((and (> (- i 1) 0) (<= (- i 1) m) (> (+ j 2) 0) (<= (+ j 2) n)) (setf list (append list (list (list (- i 1) (+ j 2))))))
				   ((and (> (+ i 1) 0) (<= (+ i 1) m) (> (- j 2) 0) (<= (- j 2) n)) (setf list (append list (list (list (+ i 1) (- j 2))))))
				   ((and (> (- i 1) 0) (<= (- i 1) m) (> (- j 2) 0) (<= (- j 2) n)) (setf list (append list (list (list (- i 1) (- j 2))))))
				   ((and (> (+ i 2) 0) (<= (+ i 2) m) (> (+ j 1) 0) (<= (+ j 1) n)) (setf list (append list (list (list (+ i 2) (+ j 1))))))
				   ((and (> (+ i 2) 0) (<= (+ i 2) m) (> (- j 1) 0) (<= (- j 1) n)) (setf list (append list (list (list (+ i 2) (- j 1))))))
				   ((and (> (- i 2) 0) (<= (- i 2) m) (> (+ j 1) 0) (<= (+ j 1) n)) (setf list (append list (list (list (- i 2) (+ j 1))))))
				   ((and (> (- i 2) 0) (<= (- i 2) m) (> (- j 1) 0) (<= (- j 1) n)) (setf list (append list (list (list (- i 2) (- j 1)))))) 
					   ) 
					   (append (list next) (check grid list m n)))))))

