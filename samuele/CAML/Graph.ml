(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** A vertex is a natural integer. *)
type vertex = int

(** An edge is an ordered pair of vertices. *)
type edge = vertex * vertex

(** A weighted edge is an edge together with a weight (a positive
 *  integer).
**)
type weighted_edge = {
    edge : edge;
    weight : int
}

(** Representation of a graph for an instance of the Steiner tree
 *  problem.
 *  Structural hypothesis :
 *      - the list terminal_vertices is sorted ;
 *      - the list weighted_edges is (lexicographically) sorted.
**)
type graph = {
    nb_vertices : int;
    terminal_vertices : vertex list;
    weighted_edges : weighted_edge list
}

(**********************************************************************)

(** nb_vertices graph : returns the number of vertices of graph. *)
let nb_vertices graph =
    graph.nb_vertices

(** vertices graph : returns the list [1 ; ... ; n] of all vertices of
 *  graph.
**)
let vertices graph =
    Tools.inter 1 (nb_vertices graph)

(** terminal_vertices graph : returns the list [1 ; ... ; n] of all
 *  terminal vertices of graph.
**)
let terminal_vertices graph =
    graph.terminal_vertices

(** nb_terminal_vertices graph : returns the number of terminal vertices
 *  of graph.
**)
let nb_terminal_vertices graph =
    List.length (terminal_vertices graph)

(** vertices_pairs graph : returns the list of all pairs of vertices of
 * graph.
**)
let vertices_pairs graph =
    let vertices = vertices graph in
    Tools.lists_product vertices vertices

(** weighted_edges graph : returns the list of all weighted edges of
 *  the graph.
**)
let weighted_edges graph =
    graph.weighted_edges

(** nb_edges graph : returns the number of edges in graph. *)
let nb_edges graph =
    List.length (weighted_edges graph)

(** edge_weight graph edge : returns the weight of the edge in the
 *  graph.
 *  Raises Not_found if the edge is not in the graph.
**)
let edge_weight graph edge =
    let weighted_edge = List.find
        (fun we -> we.edge = edge)
        (weighted_edges graph)
    in
    weighted_edge.weight

(** to_string graph : returns a string for the graph in the format
 *  of PACE 2018 (STP File Format).
**)
let to_string graph =
    let graph_data_string =
        Printf.sprintf "Nodes %d\nEdges %d\n"
            (nb_vertices graph)
            (nb_edges graph)
    in
    let edges_weight_string =
        List.fold_left (^) ""
            (List.map
                (fun v -> Printf.sprintf "E %d %d %d\n"
                    (fst v.edge) (snd v.edge) v.weight)
                (weighted_edges graph))
    in
    let terminal_data_string =
        Printf.sprintf "Terminals %d\n" (nb_terminal_vertices graph)
    in
    let terminal_vertices_string =
        List.fold_left (^) ""
            (List.map
                (fun x -> Printf.sprintf "T %d\n" x)
                (terminal_vertices graph))
    in
    Printf.sprintf
        "SECTION Graph\n%s%sEND\nSECTION Terminals\n%s%sEND\n"
        graph_data_string edges_weight_string terminal_data_string
        terminal_vertices_string

(** from_string str : returns a graph specified by the string str
 *  formated following the specifications of PACE 2018 (STP File
 *  Format).
**)
let from_string str =
    let preprocess = Str.split (Str.regexp "[ \t\n]+") str in
    let stream = Scanf.Scanning.from_string
        (String.concat " " preprocess)
    in
    let _ = Scanf.bscanf stream "SECTION Graph%s " (fun s -> None) in
    let nb_vertices = Scanf.bscanf stream "Nodes %d " (fun n -> n) in
    let nb_edges = Scanf.bscanf stream "Edges %d " (fun n -> n) in
    let rec parse_edges res =
        try
            parse_edges
            (Scanf.bscanf stream "E %d %d %d "
                (fun x y w ->
                {edge = (x, y); weight = w} :: res))
        with
            |Scanf.Scan_failure _ -> res
            |End_of_file -> res
    in
    let weighted_edges = List.sort compare (parse_edges []) in
    let _ = Scanf.bscanf stream "ND%s " (fun s -> None) in
    let _ = Scanf.bscanf stream "SECTION Terminals%s " (fun s -> None)
    in
    let nb_terminals = Scanf.bscanf stream "Terminals %d "
        (fun n -> n)
    in
    let rec parse_terminals res =
        try
            parse_terminals
            (Scanf.bscanf stream "T %d "
                (fun x -> x :: res))
        with
            |Scanf.Scan_failure _ -> res
            |End_of_file -> res
    in
    let terminal_vertices = List.sort compare (parse_terminals []) in
    let _ = Scanf.bscanf stream "END%s " (fun s -> None) in
    {
        nb_vertices = nb_vertices;
        terminal_vertices = terminal_vertices;
        weighted_edges = weighted_edges
    }
