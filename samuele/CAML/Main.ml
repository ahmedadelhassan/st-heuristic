(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

open Graph
open SteinerGraph

(**********************************************************************)
;;

print_string "==== Start program ====\n";

(** Test : graph to string. *)
let graph_1 = {
    nb_vertices = 5;
    is_terminal = (fun x -> if x = 2 || x = 4 then true else false);
    edge_value = (
        fun (x, y) -> 
            match (x, y) with
                |(1, 2) -> Some 1
                |(1, 4) -> Some 3
                |(3, 2) -> Some 3
                |(2, 4) -> Some 4
                |(3, 5) -> Some 10
                |(4, 5) -> Some 1
                |_ -> None)} 
in 
let string_1 = Graph.to_string graph_1 in
print_string string_1;


(** Test : string to graph. *)
let string_2 = 
"SECTION Graph
Nodes 3

Edges 

2
E 1 2

   1


E 3 2 2
END
SECTION Terminals

Terminals 2

T    1
T 3


END"
in
print_string string_2;
let graph_2 = Graph.from_string string_2 in
print_string (Graph.to_string graph_2);


print_string "==== End program ====\n"


