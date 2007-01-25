(defmacro setq (var value)
   (set var value))

(defun define-var (var)
   (setq (car var) (car (cdr var))))

(defmacro let (vars code)
   (set *let-temp* 
      (lamba (x)
         (seq
            (map define-var vars)
            (code))))
   (*let-temp* 0)
)

(set z 1)
(z)


(map define-var (quote ((a 1) (b 2))))
(+ a b)

(inc! z)
(inc! z)
(dec! z)
(dec! z)
