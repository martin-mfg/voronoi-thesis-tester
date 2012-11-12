import java.awt.Color;
import java.awt.Graphics2D;

public class VoronoiEdge implements Paintable {

	private static final Color[] Colors = new Color[3];
	static {
		Colors[0] = Color.RED;
		Colors[1] = Color.BLUE;
		Colors[2] = Color.BLACK;
	}

	private double[] start;

	private double[] end;

	private Color color;

	public VoronoiEdge(double[] data) {

		start = new double[2];
		end = new double[2];
		start[0] = data[0];
		start[1] = data[1];
		end[0] = data[2];
		end[1] = data[3];
		color = Colors[(int) data[4]];
	}

	@Override
	public void paintObject(Graphics2D g2) {
		g2.setColor(color);
		g2.drawLine((int) start[0], (int) start[1], (int) end[0], (int) end[1]);
		g2.setColor(Color.BLACK);
	}

	@Override
	public PaintableType getType() {
		return PaintableType.Edge;
	}

}
