;;;; Citation Counter
;;; Program a "citation counter" to evaluate candidates for tenure. The database of articles is of the following format - an article id, year, a list of authors, the title, and a list of the articles cited, e.g.,
;;; (art123 1985 (smith jones) "Sanskrit for Knowledge Representation" (art102 art256 art98))
;;; (art124 1997 (jones) "Sanskrit for Mobile Agents" (art102 art98 art123))
;;; Tenure candidates are scored by how many times articles they've written are cited by other articles, but not all citations count the same. A new citation may be worth more than an old one, a citation by a co-author worth less, and a citation to oneself worth nothing. These rules change frequently. They've developed an initial rule format and example set that your program has to support: 
;;; ((citation-by-self) => 0)
;;; ((citation-by-co-author) (citation-after 1995) => 1) ((citation-by-co-author) => 2)
;;; ((citation-after 1995) => 6)
;;; ( => 4)

;;; Eg: (citation auth1 '((art123 1990 (auth1 auth2) "topic1" (art125 art102 art103)) 
;;; (art102 1982 (auth3) "topic2" (art123 art101))
;;; (art103 1996 (auth4) "topic3" (art101 art102))
;;; (art125 1986 (auth1) "topic4" (art101 art102 art123)) 
;;; (art101 1983 (auth4 auth2) "topic5" (art102))))

;;; Output In this database if we select auth1 the row wise count 0+4+0+0+0.
;;; The total count is 4.



(defun iterate_citations (o_list list cit_list authors auth year)     ; Logic implemented with given rules   
	(cond
		(list (cond  
			((and (eql (caar list) (car cit_list)) (find auth (third (Car list))))
            (cond 
				((and (find auth authors) (find auth (third (car list)))) (if (cdr cit_list) (iterate_citations o_list o_list (cdr cit_list) authors auth year) 0))
            	((and (intersection authors (third (car list))) (> year 1995)) (if (cdr cit_list) (1+ (iterate_citations o_list o_list (cdr cit_list) authors auth year)) 1))
             	((and (intersection authors (third (car list))) (< year 1995)) (if (cdr cit_list) (+ 2 (iterate_citations o_list o_list (cdr cit_list) authors auth year)) 2))
             	((and (> year 1995) (cdr cit_list)) (+ 6 (iterate_citations o_list o_list (cdr cit_list) authors auth year)))
             	((> year 1995) 6)
             	((and (< year 1995) (cdr cit_list)) (+ 4 (iterate_citations o_list o_list (cdr cit_list) authors auth year)) )
             	((< year 1995) 4)
             	(T 0)))   
			((or (cdr cit_list) (cdr list)) (iterate_citations o_list (cdr list) cit_list authors auth year))
            (T 0))) 
		((or (cdr cit_list) (cdr list)) (iterate_citations o_list o_list (cdr cit_list) authors auth year))
   	 	(T 0)))

(defun iterate_db (auth list o_list)			; Controller function
  (if (cdr list)
        (+ (iterate_citations o_list o_list (fifth (car list)) (third (car list)) auth (second (car list))) (iterate_db auth (cdr list) o_list))
      (iterate_citations o_list o_list (fifth (car list)) (third (car list)) auth (second (car list)))
      )
  )

(defun citation (auth list) 					; Call this function to run the program
  (iterate_db auth list list))