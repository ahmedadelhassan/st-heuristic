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

let to_string e =
    Printf.sprintf "%d %d" (small e) (big e)

let from_string str =
    Scanf.sscanf str " %d %d " (fun x y -> create x y)

