(defun make-counter (x)
   (set count x)
   (lambda () (inc! count)))

(set c1 (make-counter 1))
(set c2 (make-counter 10))

(c1)
(c1)
(c2)
(c1)

(+ 1 2)
