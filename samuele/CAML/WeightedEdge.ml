(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** A weighted edge is an edge together with a weight (a positive
 *  integer).
**)
type weighted_edge = {
    edge : Edge.edge;
    weight : int
}

(**********************************************************************)

let create x y w =
    assert (x <> y);
    assert (w >= 1);
    {edge = Edge.create x y; weight = w}

let edge we = 
    we.edge

let small we =
    Edge.small we.edge

let big we =
    Edge.big we.edge

let weight we =
    we.weight

let contains_vertex we v =
    (small we) = v || (big we) = v

let other we v =
    assert (contains_vertex we v);
    Edge.other (edge we) v

let to_string we =
    Printf.sprintf "%d %d %d" (small we) (big we) (weight we)

let from_string s =
    Scanf.sscanf s " %d %d %d " (fun x y w -> create x y w)

    
    



