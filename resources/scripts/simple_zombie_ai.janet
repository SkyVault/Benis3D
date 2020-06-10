(defn vec3-dist [a b]
    (def [ax ay az] a)
    (def [bx by bz] b) 
    (math/sqrt (+ (math/pow (- bx ax) 2) 
                  (math/pow (- by ay) 2) 
                  (math/pow (- bz az) 2))))

(def *target-distance* 100.0)
(def *speed* 0.6)

(defn update[self dt] 
    (def ppos (get-player-translation)) 
    (def pos (get-translation self))

    (def [px py pz] ppos)
    (def [x y z] pos)

    (def dist (vec3-dist ppos pos))

    (if (> *target-distance* dist)
        (do
            (def angle (math/atan2 (- pz z) (- px x)))
            
            (move-entity self (* dt (math/cos angle) *speed*) 0.0 
                              (* dt (math/sin angle) *speed*)))))