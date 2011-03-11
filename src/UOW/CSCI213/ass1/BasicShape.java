import java.awt.*;
    
/**
	An abstract class for shapes.
	@version $Revision: 1.11 $
	@author $Author: dezwart $
*/
public abstract class BasicShape extends Point  {
	public BasicShape() {
		this(0, 0);
	}

	public BasicShape(double x, double y) {
		this(0, 0, "black");
	}

	public BasicShape(String color) {
		this(0, 0, color);
	}

	public BasicShape(double x, double y, String color) {
		super(x, y);
		this.color = color;
	}
    
	public void setColor(String newColor) {
		color = newColor;
	}

	public void erase() {
		Canvas canvas = Canvas.getCanvas();

		canvas.erase(this);
	}

	/**
		Makes each subclass have to impliment it's own draw method
	*/
	public abstract void draw();

	/**
		Uses a subclass' shape and draws it on a canvas.
	*/
	public void draw(Shape shape) {
		Canvas canvas = Canvas.getCanvas();

		canvas.draw(this, color, shape);
	}

	private String	color;
}
