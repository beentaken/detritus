
/*
 * The RectShape class defines a simple rectangular shape object.
 * It tracks its bounding box, selected state, and the canvas it is being
 * drawn in. It has some basic methods to select, move, and resize the
 * rectangle. It has methods that draw the shape in the selected or unselected
 * states and updates the canvas whenever the state or bounds of the rectangle
 * change. The code that is there works properly, but you will need to extend
 * and change the code to support additional features.
 *
 * @version      1.1 15/04/01
 * @author       Julie Zelenski
 */

import java.awt.*;

public class Rect extends Shape {
	/**
	* The constructor that creates a new zero width and height rectangle
	* at the given position in the canvas.
	*/

	public Rect(Point start, DrawingCanvas dcanvas, Color color) {
		super(start, dcanvas, color);
	}

	/**
	* When the DrawingCanvas needs a shape to draw itself, it sends a draw
	* message, passing the graphics context and the current region being
	* redrawn. If the shape intersects with that region, it must draw itself
	* doing whatever it takes to properly represent itself in the canvas
	* (colors, location, size, knobs, etc.) by messaging the Graphics
	* object.
	*/

	public void draw(Graphics g, Rectangle regionToDraw){
		if (!bounds.intersects(regionToDraw)) {
			return;
		}

		g.setColor(getColor());
		g.fillRect(bounds.x, bounds.y, bounds.width, bounds.height);
		drawKnobs(g);
	}

	/**
	* When the DrawingCanvas needs to determine which shape is under
	* the mouse, it asks the shape to determine if a point is "inside".
	* This method should returns true if the given point is inside the
	* region for this shape. For a rectangle, any point within the
	* bounding box is inside the shape.
	*/

	public boolean inside(Point pt){
		return bounds.contains(pt);
	}
}
