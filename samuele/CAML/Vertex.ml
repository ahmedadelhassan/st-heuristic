(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** A vertex is a natural integer. *)
type vertex = int

(**********************************************************************)

let create v =
    assert (v >= 1);
    v

let to_string v =
    Printf.sprintf "%d" v

let from_string s =
    Scanf.sscanf s " %d " (fun d -> d)

