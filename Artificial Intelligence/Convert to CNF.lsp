;;;; Conversion to Conjunctive Normal Form

;;; Program for converting propositional logic statements in to Conjunctive Normal Form. Your program needs to be able to handle logical negation, And, Or, Implication and BiCondition. 

;;; Input format
;;; A list containing a sentence in propositional logic:
;;; (convert-to-CNF l)
;;; where l is a list which containing an AtomicSentence or a ComplexSentence. AtomicSentences can either be TRUE, FALSE, P, Q, ... ComplexSentences are lists: (ComplexSentence), NOT(ComplexSentence), OR(ComplexSentence
;;; ComplexSentence), AND(ComplexSentence IF(ComplexSentence ComplexSentence). ComplexSentence) can be covered for extra credit.
	
;;; Output format
;;; A sentence in CNF.
	
;;;Sample input:
;;;(IF (OR (IF F P) (IF D P)) (IF (AND F D) P))

;;; Sample output:
;;; (AND (OR F (NOT F) (NOT D) P) (OR D (NOT D) (NOT F) P) (OR P (NOT P) (NOT F) (NOT D)))

(defun check_atoms (ret_list) 							;Check whether ret_list has atom or a list
	(cond 
		((null ret_list) t)
   	 	((atom ret_list) t)
   	 	((or (eql (car ret_list) 'or) (eql (car ret_list) 'and )) nil)   
   		(T (AND (when (OR (ATOM (FIRST RET_LIST)) (when (AND (eql (CAR (FIRST RET_LIST)) (QUOTE NOT)) (NOT (NTH 2 (FIRST RET_LIST))))T)) T) (CHECK_ATOMS (CDR RET_LIST))))))

(defun rep_n (list repl) 
	(cond  
        ((null list) ())
		(t (concatenate 'list (subseq list 0 0) repl (subseq list 1 (length list))))))

(defun rem_or_not (list symb) 							; Removes multiple or's and not's
	(cond 
		( (atom (car list)) list)
        ( (not (eql (caar list) symb)) (concatenate 'list (list (car list)) (rem_or_not (cdr list) symb )))
        ((eql (caar list) symb) (rem_or_not (rep_n  list (cdar list)) symb) )
        (T nil)))
			 
(defun funt_not_sub (listn) 
	(cond ( (atom (car listn)) 
		(concatenate 'list (list (concatenate 'list (list 'not) (list (car listn)))) (unless (null (cadr listn)) (funt_not (cdr listn))))) 
        (T (concatenate 'list  (cdar listn) (if (null (cadr listn) ) ()  (funt_not (cdr listn)))))))

(defun funt_not (listn) 								; To add not
	(cond ( (check_atoms (car listn)) (funt_not_sub listn)) 
		( T (concatenate 'list (list (cnf (concatenate 'list (list 'not) (list (cnf (car listn)))))) (if (null (cadr listn) ) ()  (funt_not (cdr listn)))))))

(defun call_recursive_cnf (crclist) 					; Call recursively CNF until all the sublist are converted
	(concatenate 'list (list (cnf (car crclist))) (unless (null (cadr crclist)) (call_recursive_cnf (cdr crclist)))))


(defun dist (opnd first list) 							; Case1 of distributive property
	(concatenate 'list (list (list opnd first (car list) )) (unless (null (cadr list)) (dist opnd first (cdr list)))))
  
(defun dist1 (opnd first list)							; Case2 of distributive property
	(concatenate 'list (list (list opnd first (car list)  )) (unless (null (cadr list) )  (dist1 opnd first (cdr list) ))))
  
(defun dist2 (opnd first list) 							; Case3 of distributive property
	(concatenate 'list (list (list opnd (car first) (car list)  )) (unless (null (cadr list)) (dist2 opnd (cdr first) (cdr list) ))))
  
(defun dist_sub_sec (opnd first rest) 					; Case4 of distributive property
	(let ((list1 (rem_or_not (dist1 opnd (car rest) (cdr first)) opnd))) (push (car first) list1)))

(defun distributive (opnd first rest) 					; Function to apply distributive property
	(cond
		((and (check_atoms first) (null (cdr first))) (let ((list1 (rem_or_not (dist opnd first (cdr rest)) opnd))) (push (car rest) list1)))
   	 	((and (check_atoms rest) (null (cdr rest))) (dist_sub_sec opnd first rest))
   	 	(T (let ((list1 (rem_or_not (dist2 opnd (cdr first) rest ) opnd))) (push (car first) list1)))))

(defun fun_for_not (list1) 								; Function to handle not
	(cond ((atom (cadr list1)) list1) 
		(T (let ((newlist (cnf (cadr list1)))) 
			(cond ((eql (car newlist) 'and) (concatenate 'list (list 'or) (funt_not (cdr newlist)))) 
				(T (concatenate 'list (list 'and) (funt_not (cdr newlist) ))))))))
				
				
(defun fun_for_iff (list1) 								; Function to handle iff
	(concatenate 'list (list 'and) (list (cnf (concatenate 'list (list 'if) (list (cadr list1)) (list (caddr list1))))) (list (cnf (concatenate 'list (list 'if) (list (caddr list1)) (list (cadr list1)))))))

(defun fun_for_if (list1) 								; Function to handle if
	(concatenate 'list (list 'or) (list (cnf (concatenate 'list (list 'not) (list (cadr list1))))) (rem_or_not (list (cnf (caddr list1))) 'or)))
	
(defun fun_for_or (list1) 								; Function to handle or
	(let ((ret_list (call_recursive_cnf (cdr list1)) ))  (if (check_atoms ret_list) list1 (distributive (car list1) (car ret_list) (cdr ret_list)))))
	
(defun fun_for_and (list1) 								; Function to handle and
	(let ((ret_list (call_recursive_cnf (cdr list1)) ))  (if (check_atoms ret_list) list1 (concatenate 'list (list 'and) ret_list))))
  
(defun CNF (list1)  									; Controller function
	(cond 
		((atom list1) list1)
   	  	((eql (car list1) 'if) (fun_for_if list1))   
   		((eql (car list1) 'not) (fun_for_not list1))
   	    ((eql (car list1) 'iff) (fun_for_iff list1))   
    	((eql (car list1) 'or)  (fun_for_or list1))
   	  	((eql (car list1) 'and)  (fun_for_and list1))
    	(T null)))
		 
(defun convert-to-cnf (list) 							; Call this function to run the program
	(if (atom list) list 
		(let ((ret (cnf list))) 
			(remove-duplicates (concatenate 'list (list (car ret)) (rem_or_not (cdr ret) (car ret)))))))

