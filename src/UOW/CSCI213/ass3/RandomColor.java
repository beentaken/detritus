
import java.awt.*;

public class RandomColor {
	public static Color MakeColor() {
		return(new Color((float)Math.random(), (float)Math.random(),
			(float)Math.random()));
	}
}
