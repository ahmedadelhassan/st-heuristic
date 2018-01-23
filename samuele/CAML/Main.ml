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
    terminal_vertices = [2; 4];
    weighted_edges = [
        {edge = (1, 2); weight = 1};
        {edge = (1, 4); weight = 3};
        {edge = (3, 2); weight = 3};
        {edge = (2, 4); weight = 4};
        {edge = (3, 5); weight = 10};
        {edge = (4, 5); weight = 1};
    ]
}
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


(** Test : file to graph. *)

let file_1 = open_in "../../public/instance083.gr" in
let string_3 = really_input_string file_1 (in_channel_length file_1) in
let graph_3 = Graph.from_string string_3 in
print_string (Graph.to_string graph_3);

print_string "==== End program ====\n"
