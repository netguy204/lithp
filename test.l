(* (+ 2 4) 2)
(set b 2)
(* b b)

(square 2)
(square 4)
(square 6)

(set lista (quote (1 2 3 4)))
(car lista)
(cdr lista)
(cdr (cdr lista))
(cdr (cdr (cdr lista)))
()

(set mac (macro (e f) (+ e f)))
(mac 1 2)

(defmacro add (e f) (+ e f))
(add 1 2)

(= 1 1)
(= 1 2)

(+ 1 -1)
(cons 3 (cons 1 (cons 2 ())))
(cons 1 2)
(square (car (cdr lista)))
(map square lista)

(set t (lambda () (+ 1 2)))
(t)
