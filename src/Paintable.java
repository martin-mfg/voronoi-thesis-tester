import java.awt.Graphics2D;

public interface Paintable {

	public static enum PaintableType {
		Point, Edge;
	}

	public PaintableType getType();

	public void paintObject(Graphics2D g2);
}