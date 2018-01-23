(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

open Graph

(** A Steiner graph is made of a ground graph, a subset of vertices,
 *  and a subset of edges.
 *  Structural hypothesis :
 *      - the list vertices is sorted ;
 *      - the list edges is (lexicographically) sorted.
 *  For the given problem, we are looking for Steiner graphs that are
 *  trees and that contain all terminal vertices.
**)
type steiner = {
    graph : Graph.graph;
    vertices : Graph.vertex list;
    edges : Graph.edge list
}

(**********************************************************************)

(** contains_all_terminal_vertices steiner : tests if the steiner graph
 *  contains all terminal vertices of the ground graph.
**)
let contains_all_terminal_vertices steiner =
    List.for_all
        (fun x -> List.exists (fun y -> y = x) steiner.vertices)
        (Graph.terminal_vertices steiner.graph)

(** is_tree steiner : tests if the steiner graph is a tree.
 *  TO DO.
**)
let is_tree steiner =
    false

(** is_valid steiner : tests if the steiner graph is valid (that is, a
 *  tree containing all terminal vertices.
** *)
let is_valid steiner =
    (contains_all_terminal_vertices steiner) && (is_tree steiner)

(** weight steiner : returns the weight of the steiner graph. *)
let weight steiner =
    List.fold_left
        (fun w e -> w + (Graph.edge_weight steiner.graph e))
        0
        steiner.edges

(** generate_random ground_graph : returns a steiner graph on the
 *  ground graph generated at random (each edge has a fixed probability
 *  to be appear.
 *  TO DO.
**)
let generate_random ground_graph =
    ()

(** random_flip steiner : returns a steiner graph obtained by performing
 *  randomly a flip operation on the steiner graph.
 *  TO DO.
**)
let random_flip steiner =
    ()

let force_include_all_terminal_vertices steiner =
    ()

let force_be_a_tree steiner =
    ()
