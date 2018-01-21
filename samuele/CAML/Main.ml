(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

open Graph
open SteinerGraph

(**********************************************************************)
;;

print_string "Start\n";

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




print_string "End\n"


