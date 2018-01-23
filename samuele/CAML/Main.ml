(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(**********************************************************************)
;;

print_string "==== Start program ====\n";

(** Test: graph to string. *)
let graph_1 = Graph.create
    5
    [2; 4]
    [
        WeightedEdge.create 1 2 1;
        WeightedEdge.create 1 4 3;
        WeightedEdge.create 3 2 3;
        WeightedEdge.create 2 4 4;
        WeightedEdge.create 3 5 10;
        WeightedEdge.create 4 5 1
    ]
in
let string_1 = Graph.to_string graph_1 in
print_string string_1;


(** Test: string to graph. *)
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


(** Test: file to graph. *)

let file_1 = open_in "../../public/instance039.gr" in
let string_3 = really_input_string file_1 (in_channel_length file_1) in
let graph_3 = Graph.from_string string_3 in
print_string (Graph.to_string graph_3);

(** Test: random steiner graph. *)
let steiner_1 = SteinerGraph.generate_random graph_3 16 in 
print_string (SteinerGraph.to_string steiner_1);



print_string "==== End program ====\n"








