# Polygon Editor with Constraints

This repository contains a Qt-based polygon editor application that allows users to interactively create, edit, and constrain polygons. The system leverages a hierarchical object structure to represent and manipulate polygon vertices, edges, and their associated constraints. It is designed to be both extensible and illustrative of various computer graphics and computational geometry concepts.

## Key Features

- **Polygon Creation and Editing:**  
  Users can create polygons by adding vertices. Vertices are represented as specialized Qt graphics items that can be freely moved around the canvas. Edges are automatically maintained to ensure closed polygonal structures.

- **Edge Types:**  
  There are normal straight line edges and Bezier edges. A normal edge is defined simply by its start and end vertices. A Bezier edge introduces two additional control vertices, making it possible to create smooth curves rather than straight lines.

- **Constraints System:**  
  Edges and vertices can be assigned constraints to enforce geometric relationships. Current constraints include:
  - Horizontal and Vertical edge constraints to keep edges aligned along the axes.
  - Length constraints to maintain a specific edge length.
  - C1 and G1 vertex continuity constraints for Bezier edges.  
    These constraints ensure smooth curvature and directionality at the vertices connecting two Bezier edges.
  
  When constraints are applied, the system uses the `ConstraintChecker` utility to propagate and enforce these constraints throughout the polygon.

- **Bresenham's Line Drawing:**  
  Edges can be drawn using the traditional Bresenham algorithm for a pixelated, low-level computer graphics feel. This can be toggled, allowing for a switch between anti-aliased line rendering and Bresenham drawing, to better illustrate the algorithm in action.

- **Context Menus for Editing:**  
  Right-clicking on edges or vertices displays context menus. Users can add or remove constraints, switch edge types between normal and Bezier, and subdivide edges by inserting new vertices.

- **Move-All-Vertices Mode:**  
  A mode allowing the entire polygon to be moved at once, demonstrating hierarchical transformations and item grouping in a scene.

## File Structure

- **include/**  
  Contains header files for key classes:
  - `PolygonItem.h`: Main class representing the polygon, holding vertices and edges.  
  - `Vertex/PolygonVertexItem.h` and `Vertex/BaseVertexItem.h`: Classes representing vertices.  
  - `Edge/PolygonEdgeItem.h`, `Edge/BezierEdgeItem.h`, etc.: Classes for different edge types.  
  - `Constraints/*.h`: Constraint classes that enforce geometric relationships.

- **src/**  
  Contains the corresponding implementation files that define the behavior declared in the headers.

- **MainWindow.h / MainWindow.cpp**  
  A simple UI for managing the polygon scene. Includes toolbars and a canvas to display and interact with the polygon.

## How It Works

1. **Initialization:**  
   On startup, a default polygon is created. A `QGraphicsScene` and `QGraphicsView` are set up, and `PolygonItem` is added to the scene.

2. **Adding Vertices and Edges:**  
   Clicking the "Add Vertex" button randomly places a new vertex in the scene and connects it to form or extend a polygon. Closing edges are automatically created to ensure a proper polygonal shape.

3. **Applying Constraints:**  
   Right-click an edge or vertex and apply various constraints from the context menu. Constraints are applied using the `ConstraintChecker` utility, ensuring the polygon remains valid and consistent with all active constraints.

4. **Editing & Interaction:**  
   Dragging vertices updates edges and constraints in real-time. Subdividing edges creates new vertices for finer polygon modeling. Switching edges between normal and Bezier forms allows exploring different curve representations.

## Installation and Building

1. **Prerequisites:**  
   - Qt 6 (with Qt Widgets)
   - A C++17 compatible compiler
   - CMake (optional, depending on your build setup)

2. **Building:**  
   ```  
   mkdir build && cd build  
   cmake ..  
   make  
   ./PolygonEditor  
   ```

   This will launch the application, showing the main window with toolbars and a canvas where a default polygon is displayed.
