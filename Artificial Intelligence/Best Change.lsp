;;;; Best Change Program

;;; Program that can determine the best exact change for a certain set of coins, where best means that it returns a solution which leaves the least amount of cents unaccounted for and uses the fewest number of coins.

;;; Input format
;;; An integer for the change amount requested and list containing the coins available: (find-best-change n ‘(c1 c2 ... cm))
;;; where n is the amount of change requested and c1 c2 ... cm are the set of coins available.
;;; (find-best-change 30 '(25 10 1))

;;; Output format
;;; ((x1 c1) (x2 c2) ... (xz cz))

;;; Example
;;; Input1 : (find-best-change 30 '(25 10 1))
;;; Output1 : ((0 25) (3 10) (0 1))
;;; Input1 : (find-best-change 29 '(26 10 5 1))
;;; Output1 : ((1 26) (0 10) (0 5) (3 1))



(defun call_fun (coins)											;This function adds 0's to the list coins
  (if (endp coins) () (append '(0) (call_fun (cdr coins)))))
  

(defun best_c (change list)										;Computes number of coins for diffent denomination combinations
  (if (endp list) () 
    (if (= change 0) (call_fun list)
       (if (> (car list) change)
              (append (list 0) (best_c change (cdr list)))
            (progn
             (setf div (floor (/ change (car list))))
              (setf coins_left (mod change (car list)))
             (append (list div) (best_c coins_left (cdr list)))
             )
           )
         )
    )
  )
  
(defun compute_sum (list)										;Function which computes sum of elements of list
	(if (endp list) 0
		(+ (car list) (compute_sum (cdr list)))))
					  
(defun add_zero (i var)											;Appends i number of 0's to var 
  (if (> i 0) (append var (append '(0) (add_zero (- i 1) var))) ()))
  
(defun compute_rem (var1 list12 change12 rem)					;Computes remainder for a particular set of coins
   (loop for j from 0 to (- (list-length list12) 1) do
        (setf rem (setf  change12 (- change12 (* (car list12) (car var1)))))(pop list12)(pop var1)
         ) rem 
 )


(defun loop_b_main (change list list_len n list1 change1)		;Computes number of coins for each denomination and replaces the result if the next denomination uses less coins
  (if (> list_len 0)
      (progn        
        (setf var (add_zero (- n list_len) '()))
        (setf var (append var (best_c change list)))
        (setf rem (compute_rem var list1 change1 0))
        (push (compute_sum var) var)
         (push rem var)        
        (pop list)
        (if  (>= (car final_look_up) (car var)) 				;replacing the stored set of coins with the new set which has less number of coins 
            (progn (if  (and (<= (cadr final_look_up) (cadr var)) (< (car var) (car final_look_up)))                     
                        (setf final_look_up var) )
              (if (> (cadr final_look_up) (cadr var)) (setf final_look_up var))
              ))
        (loop_b_main change list (- list_len 1) n list1 change1)
         final_look_up
        )
    )
  )



(defun find-best-change ( change list)							;This function needs to be called for execution of the program
	(setf list (sort list #'>))
	(defparameter final_look_up '())
  	(defparameter rem '())
 	(push (floor (/ change (loop for x in list minimize x))) final_look_up)
  	(push (loop for x in list maximize x) final_look_up)
  	(setf result (loop_b_main change list (list-length list) (list-length list) list change))
  	(pop result) (pop result)
  	(setf end_res '())
  	(if (equal result nil) (setf end_res (list 0 (car list))) 	;Formatting for the output
  	  (loop for j from 0 to (- (list-length list) 1) do
      	  (setf end_res (append end_res (list (list (nth j result) (nth j list))))))
    )
	end_res                                     
  )