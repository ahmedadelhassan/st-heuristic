(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** A Steiner graph is made of a ground graph, a subset of vertices,
 *  and a subset of edges.
 *  Structural hypothesis:
 *      - the list edges is (lexicographically) sorted;
 *      - all the edges of the list are weighted edges in the graph.
 *  For the given problem, we are looking for Steiner graphs that are
 *  trees and that contain all terminal vertices.
**)
type steiner = {
    graph : Graph.graph;
    edges : Edge.edge list
}

(**********************************************************************)

(** contains_all_terminal_vertices steiner : tests if the steiner graph
 *  contains all terminal vertices of the ground graph.
**)
let contains_all_terminal_vertices steiner =
    List.for_all
        (fun x -> 
            (List.exists 
                (fun e -> (Edge.small e) = x || (Edge.big e) = x) 
                steiner.edges))
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
 *  to appear). The probability of each edge is 1 / p.
**)
let generate_random ground_graph p =
    assert (p >= 1);
    let edges = 
        Tools.filtered_map 
            (fun we -> (Random.int 4096) mod p = 0) 
            WeightedEdge.edge
            (Graph.weighted_edges ground_graph)
    in
    {
        graph = ground_graph;
        edges = edges
    }

(** random_flip steiner : returns a steiner graph obtained by performing
 *  randomly a flip operation on the steiner graph.
 *  TO DO.
**)
let random_flip steiner =
    ()

let force_include_all_terminal_vertices steiner =
    ()

(** Prim or Kruskal. *)
let force_be_a_tree steiner =
    ()

let to_string steiner = 
    let edges_string =
        List.fold_left (^) ""
            (List.map
                (fun e -> Printf.sprintf "%s\n" (Edge.to_string e)) 
                steiner.edges)
    in
    Printf.sprintf "VALUE %d\n%s" (weight steiner) edges_string

