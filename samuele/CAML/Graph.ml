(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** Representation of a graph for an instance of the Steiner tree
 *  problem.
 *  Structural hypothesis:
 *      - the list terminal_vertices is sorted;
 *      - all the vertices of terminal_vertices are between 1 and 
 *        nb_vertices;
 *      - the list weighted_edges is (lexicographically) sorted;
 *      - all the vertices of weighted_edges are between 1 and 
 *        nb_vertices.
**)
type graph = {
    nb_vertices : int;
    terminal_vertices : Vertex.vertex list;
    weighted_edges : WeightedEdge.weighted_edge list
}

(**********************************************************************)

exception Not_an_edge
exception Parsing_error

(**********************************************************************)

(** create n t we : returns the graph consisting in `n` vertices, the
 *  list `t` of terminal vertices, and the the list `we` of weighted 
 *  edges.
**)
let create n t we = 
    assert (n >= 0);
    {nb_vertices = n; terminal_vertices = t; weighted_edges = we}

(** nb_vertices g : returns the number of vertices of the graph `g`. *)
let nb_vertices g =
    g.nb_vertices

(** vertices g : returns the list `[1; ... ;n]` of all the vertices of
 *  graph `g` having `n` vertices.
**)
let vertices g =
    Tools.inter 1 (nb_vertices g)

(** terminal_vertices g : returns the sorted list of all the terminal
 *  vertices of the graph `g`.
**)
let terminal_vertices g =
    g.terminal_vertices

(** nb_terminal_vertices g : returns the number of terminal vertices
 *  of the graph `g`.
**)
let nb_terminal_vertices g =
    List.length (terminal_vertices g)

(** vertices_pairs g : returns the list of all pairs of vertices of
 *  the graph `g`. This list includes all possible edges, even if they
 *  do not belong to `g`.
**)
let vertices_pairs g =
    let vertices = vertices g in
    Tools.lists_product vertices vertices

(** weighted_edges g : returns the list of all weighted edges of the
 *  graph `g`.
**)
let weighted_edges g =
    g.weighted_edges

(** nb_edges g : returns the number of weighted edges in the graph `g`. *)
let nb_edges g =
    List.length (weighted_edges g)

(** edge_weight g e : returns the weight of the edge `e` in the graph 
 * `g`. Raises `Not_an_edge` if `e` is not in `g`.
**)
let edge_weight g e =
    try
        let weighted_edge = List.find
            (fun we -> (WeightedEdge.edge we) = e)
            (weighted_edges g)
        in
        WeightedEdge.weight weighted_edge
    with
        |_ -> raise Not_an_edge

(** is_edge g e : tests if the edge `e` is in the graph `g`. *)
let is_edge g e =
    try 
        let _ = edge_weight g e in
        true
    with
        |Not_an_edge -> false

(** adjacent_vertices g v : returns the sorted list of all vertices
 *  adjacent to the vertex `v` in the graph `g`.
**)
let adjacent_vertices g v =
    let vertices = 
        Tools.filtered_map 
            (fun we -> WeightedEdge.contains_vertex we v)
            (fun we -> WeightedEdge.other we v)
            (weighted_edges g)
    in
    Tools.list_suppress_runs (List.sort compare vertices)

(** to_string g : returns a string for the graph `ŋ` in following the 
 *  STP File Format.
**)
let to_string g =
    let graph_data_string =
        Printf.sprintf "Nodes %d\nEdges %d\n"
            (nb_vertices g)
            (nb_edges g)
    in
    let edges_weight_string =
        List.fold_left (^) ""
            (List.map
                (fun we -> Printf.sprintf "E %s\n" 
                    (WeightedEdge.to_string we)) 
                (weighted_edges g))
    in
    let terminal_data_string =
        Printf.sprintf "Terminals %d\n" (nb_terminal_vertices g)
    in
    let terminal_vertices_string =
        List.fold_left (^) ""
            (List.map
                (fun x -> Printf.sprintf "T %s\n" (Vertex.to_string x))
                (terminal_vertices g))
    in
    Printf.sprintf
        "SECTION Graph\n%s%sEND\nSECTION Terminals\n%s%sEND\n\nEOF\n"
        graph_data_string edges_weight_string terminal_data_string
        terminal_vertices_string

(** from_string s : returns a graph specified by the string `s` formated
 *  following the STP File Format. Raises `Parsing_error` if `s` is not
 *  a valid string.
**)
let from_string s =
    let preprocess = Str.split (Str.regexp "[ \t\n]+") s in
    let stream = Scanf.Scanning.from_string
        (String.concat " " preprocess)
    in
    try 
        let _ = Scanf.bscanf stream "SECTION Graph%s " (fun s -> None) 
        in
        let n = Scanf.bscanf stream "Nodes %d " (fun n -> n) in
        let _ = Scanf.bscanf stream "Edges %d " (fun n -> None) in
        let rec parse_edges res =
            try
                parse_edges
                (Scanf.bscanf stream "E %d %d %d "
                    (fun x y w -> (WeightedEdge.create x y w) :: res))
            with
                |Scanf.Scan_failure _ -> res
                |End_of_file -> res
        in
        let we = List.sort compare (parse_edges []) in
        let _ = Scanf.bscanf stream "ND%s " (fun s -> None) in
        let _ = Scanf.bscanf stream "SECTION Terminals%s " 
            (fun s -> None)
        in
        let _ = Scanf.bscanf stream "Terminals %d " (fun n -> None) in
        let rec parse_terminals res =
            try
                parse_terminals
                (Scanf.bscanf stream "T %d "
                    (fun x -> (Vertex.create x) :: res))
            with
                |Scanf.Scan_failure _ -> res
                |End_of_file -> res
        in
        let t = List.sort compare (parse_terminals []) in
        let _ = Scanf.bscanf stream "END%s " (fun s -> None) in
        let _ = Scanf.bscanf stream "EOF%s " (fun s -> None) in
        {nb_vertices = n; terminal_vertices = t; weighted_edges = we}
    with
        |_ -> raise Parsing_error

