(set defun
   (macro (name args code)
      (set name (lambda args (seq code)))))

(set defmacro
   (macro (name args code)
      (set name (macro args code))))

(defun inc (x) (+ x 1))
(defun dec (x) (+ x -1))

(defun map (l m)
   (cond (cdr m)
      (cons (l (car m)) (map l (cdr m)))
      (cons (l (car m)) ())))

(defun square (x) (* x x))

(defmacro inc! (x) (set x (+ x 1)))

(defmacro dec! (x) (set x (+ x -1)))

