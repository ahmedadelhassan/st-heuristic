(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** An edge is an ordered pair of vertices. 
 *  Structural hypothesis:
 *      - the vertices are both positive integers;
 *      - the first vertex is smaller than the second. 
**)
type edge = Vertex.vertex * Vertex.vertex

(**********************************************************************)

let create x y =
    assert (x <> y);
    ((min x y), (max x y))

let small e =
    fst e

let big e =
    snd e

let contains_vertex e v =
    (small e) = v || (big e) = v

let other e v =
    assert (contains_vertex e v);
    if (small e) = v then
        big e
     else
        small e

let to_string e =
    Printf.sprintf "%d %d" (small e) (big e)

let from_string s =
    Scanf.sscanf s " %d %d " (fun x y -> create x y)

