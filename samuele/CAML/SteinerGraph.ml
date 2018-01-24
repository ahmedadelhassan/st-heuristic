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

let create g e =
    {graph = g; edges = e}

let graph s = 
    s.graph

let edges s =
    s.edges

(** vertices s : returns the list of all the vertices of the Steiner
 *  graph `s`. These are the vertices of the ground graph of `s` that
 *  are adjacent to at least one edge in `s`.
**)
let vertices s =
    let vertices = 
        List.fold_left 
            (fun l e -> (Edge.small e) :: (Edge.big e) :: l)
            []
            (edges s)
    in
    Tools.list_suppress_runs (List.sort compare vertices)

(** contains_all_terminal_vertices s : tests if the Steiner graph `s`
 *  contains all terminal vertices of its ground graph.
**)
let contains_all_terminal_vertices s =
    List.for_all
        (fun x -> 
            (List.exists (fun e -> Edge.contains_vertex e x) s.edges))
        (Graph.terminal_vertices s.graph)

(** adjacent_vertices s v : returns the sorted list of all vertices
 *  adjacent to the vertex `v` in the Steiner graph `s`.
**)
let adjacent_vertices s v =
    let vertices = 
        Tools.filtered_map 
            (fun e -> Edge.contains_vertex e v)
            (fun e -> Edge.other e v)
            (edges s)
    in
    Tools.list_suppress_runs (List.sort compare vertices)

(** is_connex s : tests if the Steiner graph `s` is connex.
 *  NOT EFFICIENT.
**)
let is_connex s =
    let uf =
        List.fold_left 
            (fun uf e -> UnionFind.union uf (Edge.small e) (Edge.big e))
            (List.map (fun v -> [v]) (vertices s))
            (edges s)
    in
    match uf with
        |[] -> true
        |[cl] -> true
        |_ -> false

(** is_acyclic s : tests if the Steiner graph `s` is acyclic.
 *  NOT EFFICIENT.
**)
let is_acyclic s = 
    let rec treat_edges uf edges =
        match edges with 
            |[] -> true
            |e :: tail ->
                let x = Edge.small e and y = Edge.big e in
                if UnionFind.are_in_same_class uf x y then
                    false
                else
                    treat_edges (UnionFind.union uf x y) tail
    in
    treat_edges (List.map (fun v -> [v]) (vertices s)) (edges s)

(** is_tree s : tests if the Steiner graph `s` is a tree. *)
let is_tree s =
    (is_acyclic s) && (is_connex s)

(** is_valid s : tests if the Steiner graph `s` is valid (that is, a
 *  tree containing all terminal vertices of its ground graph).
**)
let is_valid s =
    (contains_all_terminal_vertices s) && (is_tree s)

(** weight s : returns the weight of the Steiner graph `s`. *)
let weight s =
    List.fold_left
        (fun w e -> w + (Graph.edge_weight s.graph e))
        0
        s.edges

(** generate_random g p : returns a Steiner graph on the ground graph 
*   `g` generated at random. Each edge has a fixed `1 / p` probability 
*   to appear.
**)
let generate_random g p =
    assert (p >= 1);
    let edges = 
        Tools.filtered_map 
            (fun we -> (Random.int 4096) mod p = 0) 
            WeightedEdge.edge
            (Graph.weighted_edges g)
    in
    {graph = g; edges = edges}

(** random_flip s : returns a Steiner graph obtained by performing
 *  randomly a flip operation on the Steiner graph `s`.
 *  TO DO.
**)
let random_flip steiner =
    ()

let force_include_all_terminal_vertices steiner =
    ()

(** Prim or Kruskal. 
 *  TO DO
**)
let force_be_a_tree steiner =
    ()

let to_string s = 
    let edges_string =
        List.fold_left (^) ""
            (List.map
                (fun e -> Printf.sprintf "%s\n" (Edge.to_string e)) 
                s.edges)
    in
    Printf.sprintf "VALUE %d\n%s" (weight s) edges_string

