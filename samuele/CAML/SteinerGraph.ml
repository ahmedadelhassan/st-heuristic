(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

open Graph

type steiner = {
    graph : Graph.graph;
    contains_vertex : Graph.vertex -> bool;
    contains_edge : Graph.edge -> bool
}

(**********************************************************************)

let contains_all_terminals steiner =
    List.for_all steiner.contains_vertex (Graph.terminal_vertices steiner.graph)

let is_tree steiner =
    ()

let is_valid steiner = 
    ()

let weight steiner =
    ()

let generate_random ground_graph =
    ()

let random_flip steiner =
    ()
