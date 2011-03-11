import java.awt.Dimension;
import javax.swing.JComponent;
import javax.swing.JPanel;

/**
	A GoKart that uses Boxes and Circles
	@version $Revision: 1.25 $
	@author $Author: dezwart $
*/
public class GoKart extends Point {
	public GoKart() {
		this(0, 0);
	}

	/**
		Initialises a GoKart and sets up the component objects
	*/
	public GoKart(double x, double y) {
		super(x, y);

		x_vector = -1;
		y_vector = 0;

		int i = 0;

		shapes = new BasicShape[num_shapes];

		front_wheel = new Circle(x, y, "green", WHEEL_RADIUS);
		shapes[i++] = front_wheel;

		back_wheel = new Circle(x + WHEEL_BASE, y, "red",
			WHEEL_RADIUS);
		shapes[i++] = back_wheel;

		chassis = new Box(x, y, "black", CHASSIS_LENGTH, LINE_WIDTH);
		shapes[i++] = chassis;

		seat = new Box(x + WHEEL_BASE - WHEEL_RADIUS, y - WHEEL_RADIUS,
			"blue", 2 * WHEEL_RADIUS, WHEEL_RADIUS);
		shapes[i++] = seat;
	}

	/**
		Draws all the component objects.
	*/
	public void draw() {
		for (int i = 0; i < num_shapes; i++) {
			shapes[i].draw();
		}
		
		Canvas foo = Canvas.getCanvas();
		foo.wait(15);
	}

	/**
		Erases all the component objects.
	*/
	public void erase() {
		for (int i = 0; i < num_shapes; i++) {
			shapes[i].erase();
		}
	}

	/**
		_move's all component objects then erases and draws them.
	*/
	public void move(double x, double y) {
		_move(x, y);

		//erase();
		draw();
	}

	/**
		Moves the GoKart forward around the perimeter of the Canvas.
	*/
	public void goAroundForward(double speed) {
		while (speed > 0) {
			// Check to see that the GoKart does not go past the
			// perimeter.
			if (x_vector > 0) {
				if ((X() + speed + CHASSIS_LENGTH +
					WHEEL_RADIUS + LINE_WIDTH)
					> Canvas.WIDTH) {
					turnLeft();
				}
			} else if (x_vector < 0) {
				if ((X() - speed - WHEEL_RADIUS -
					LINE_WIDTH) < 0) {
					turnLeft();
				}
			} else if (y_vector > 0) {
				if ((Y() + speed + CHASSIS_LENGTH +
					WHEEL_RADIUS + LINE_WIDTH)
					> Canvas.HEIGHT) {
					turnLeft();
				}
			} else {
				if ((Y() - speed - WHEEL_RADIUS - LINE_WIDTH)
					< 0) {
					turnLeft();
				}
			}

			go(speed);
		}
	}

	/**
		Moves the GoKart backward around the perimeter of the Canvas.
	*/
	public void goAroundBackward(double speed) {
		while (speed > 0) {
			// Check to see that the GoKart does not go past
			// the perimeter
			if (x_vector > 0) {
				if ((X() - speed - WHEEL_RADIUS - LINE_WIDTH)
					< 0) {
					turnRight();
				}
			} else if (x_vector < 0) {
				if ((X() + speed + CHASSIS_LENGTH +
					WHEEL_RADIUS + LINE_WIDTH)
					> Canvas.WIDTH) {
					turnRight();
				}
			} else if (y_vector > 0) {
				if ((Y() - speed - WHEEL_RADIUS - LINE_WIDTH)
					< 0) {
					turnRight();
				}
			} else {
				if ((Y() + speed + CHASSIS_LENGTH +
					WHEEL_RADIUS + LINE_WIDTH)
					> Canvas.HEIGHT) {
					turnRight();
				}
			}

			go(-speed);
		}
	}

	/**
		Moves the GoKart STATIC_MOVE pixes Forward.
	*/
	public void goForward() {
		go(STATIC_MOVE);
	}

	/**
		Moves the GoKart STATIC_MOVE pixels Backward.
	*/
	public void goBackward() {
		go(-STATIC_MOVE);
	}
		
	/**
		Turns the GoKart 90 degrees to the Left. (-pi/2 radians shift)
	*/
	public void turnLeft() {
		if (x_vector < 0) {
			front_wheel.move(0, WHEEL_BASE);
			back_wheel.move(-WHEEL_BASE, 0);
			chassis.setDimensions(LINE_WIDTH, CHASSIS_LENGTH);
			seat.move(-WHEEL_BASE, 2 * WHEEL_RADIUS);
			seat.setDimensions(WHEEL_RADIUS, 2 * WHEEL_RADIUS);

			x_vector = 0;
			y_vector = 1;
		} else if (x_vector > 0) {
			front_wheel.move(0, -WHEEL_BASE);
			back_wheel.move(WHEEL_BASE, 0);
			chassis.move(CHASSIS_LENGTH, -CHASSIS_LENGTH);
			chassis.setDimensions(LINE_WIDTH, CHASSIS_LENGTH);
			seat.move(WHEEL_BASE + WHEEL_RADIUS, 3 * -WHEEL_RADIUS);
			seat.setDimensions(WHEEL_RADIUS, 2 * WHEEL_RADIUS);

			x_vector = 0;
			y_vector = -1;
		} else if (y_vector < 0) {
			front_wheel.move(-WHEEL_BASE, 0);
			back_wheel.move(0, -WHEEL_BASE);
			chassis.move(-CHASSIS_LENGTH, 0);
			chassis.setDimensions(CHASSIS_LENGTH, LINE_WIDTH);
			seat.move(-3 * WHEEL_RADIUS, -WHEEL_BASE);
			seat.setDimensions(2 * WHEEL_RADIUS, WHEEL_RADIUS);

			y_vector = 0;
			x_vector = -1;
		} else if (y_vector > 0) {
			front_wheel.move(WHEEL_BASE, 0);
			back_wheel.move(0, WHEEL_BASE);
			chassis.move(0, CHASSIS_LENGTH);
			chassis.setDimensions(CHASSIS_LENGTH, LINE_WIDTH);
			seat.move(2 * WHEEL_RADIUS, WHEEL_BASE + WHEEL_RADIUS);
			seat.setDimensions(2 * WHEEL_RADIUS, WHEEL_RADIUS);

			y_vector = 0;
			x_vector = 1;
		}

		erase();
		draw();
	}

	/**
		Turns the GoKart 90 degrees to the Right. (pi/2 radians shift)
	*/
	public void turnRight() {
		if (y_vector < 0) {
			front_wheel.move(0, WHEEL_BASE);
			back_wheel.move(-WHEEL_BASE, 0);
			chassis.move(-CHASSIS_LENGTH, CHASSIS_LENGTH);
			chassis.setDimensions(CHASSIS_LENGTH, LINE_WIDTH);
			seat.move(-(WHEEL_BASE + WHEEL_RADIUS),
				3 * WHEEL_RADIUS);
			seat.setDimensions(2 * WHEEL_RADIUS, WHEEL_RADIUS);

			y_vector = 0;
			x_vector = 1;
		} else if (y_vector > 0) {
			front_wheel.move(0, -WHEEL_BASE);
			back_wheel.move(WHEEL_BASE, 0);
			chassis.setDimensions(CHASSIS_LENGTH, LINE_WIDTH);
			seat.move(WHEEL_BASE, 2 * -WHEEL_RADIUS);
			seat.setDimensions(2 * WHEEL_RADIUS, WHEEL_RADIUS);

			y_vector = 0;
			x_vector = -1;
		} else if (x_vector < 0) {
			front_wheel.move(WHEEL_BASE, 0);
			back_wheel.move(0, WHEEL_BASE);
			chassis.move(CHASSIS_LENGTH, 0);
			chassis.setDimensions(LINE_WIDTH, CHASSIS_LENGTH);
			seat.move(3 * WHEEL_RADIUS, WHEEL_BASE);
			seat.setDimensions(WHEEL_RADIUS, 2 * WHEEL_RADIUS);

			x_vector = 0;
			y_vector = -1;
		} else if (x_vector > 0) {
			front_wheel.move(-WHEEL_BASE, 0);
			back_wheel.move(0, -WHEEL_BASE);
			chassis.move(0, -CHASSIS_LENGTH);
			chassis.setDimensions(LINE_WIDTH, CHASSIS_LENGTH);
			seat.move(-2 * WHEEL_RADIUS, -6 * WHEEL_RADIUS);
			seat.setDimensions(WHEEL_RADIUS, 2 * WHEEL_RADIUS);

			x_vector = 0;
			y_vector = 1;
		}

		erase();
		draw();
	}

	// Simple BasicShape array for doing iterative operations
	private BasicShape[]	shapes;

	private Circle		back_wheel;
	private Circle		front_wheel;
	private Box		chassis;
	private Box		seat;

	private int		x_vector;
	private int		y_vector;

	// Class wide constants
	private static final int	num_shapes = 4;
	private static final double	LINE_WIDTH = 1;
	private static final double	WHEEL_RADIUS = 10;
	private static final double	WHEEL_BASE = 5 * WHEEL_RADIUS;
	private static final double	CHASSIS_LENGTH = 7 * WHEEL_RADIUS;
	private static final double	STATIC_MOVE = 20;

	/**
		Moves all the component objects without redrawing each one.
	*/
	private void _move(double x, double y) {
		super.move(x, y);

		for (int i = 0; i < num_shapes; i++) {
			shapes[i].move(x, y);
		}
	}

	/**
		Moves the GoKart along the specified vectors at a set speed.
	*/
	private void go(double speed) {
		move(x_vector * speed, y_vector * speed);
	}
}
