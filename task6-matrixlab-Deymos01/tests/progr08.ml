(= H0 1)
(= H1 (vertcat (horzcat H0 H0) (horzcat H0 (- H0))))
(= H2 (vertcat (horzcat H1 H1) (horzcat H1 (- H1))))
(= H4 (vertcat (horzcat H2 H2) (horzcat H2 (- H2))))
(= Q (* (det H4) (inv H4)))
(= result (sum (- Q (* 512 H4))))
(disp result)
(disp (sum (- (* H4 H4) (* H4 (transpose H4)))))
