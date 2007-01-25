(defun make-list (start end)
   (cond (= start end)
      (cons end ())
      (cons start (make-list (+ start 1) end))))

(defun fact (x)
   (cond (= x 0)
      1
      (* x (fact (+ x -1)))))

(map square (make-list 1 10))
(map fact (make-list 1 10))

(set add +)
(add 1 2 3)

(fact 4)
