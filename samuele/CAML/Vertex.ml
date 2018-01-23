(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** A vertex is a natural integer. *)
type vertex = int

(**********************************************************************)

let to_string v =
    Printf.sprintf "%d" v

let from_string str =
    Scanf.sscanf str " %d " (fun d -> d)

