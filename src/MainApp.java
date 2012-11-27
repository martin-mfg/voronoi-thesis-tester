import java.awt.Dimension;
import java.util.Arrays;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

public class MainApp extends JFrame {

	private static MainApp instance;

	public static MainApp getInstance() {
		if (instance == null) {
			instance = new MainApp();
		}
		return instance;
	}

	public static void main(String[] args) {

		SwingUtilities.invokeLater(new Runnable() {

			@Override
			public void run() {
				// JNITest.test();
				MainApp.getInstance().startProgram();
			}
		});
	}

	private MainApp() {
		super("Voronoi Editor");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	private void startProgram() {
		// setLookAndFeel();
		showGUI();
	}

	private void setLookAndFeel() {
		try {
			System.out.println(Arrays.toString(UIManager
					.getInstalledLookAndFeels()));
			Class.forName("javax.swing.plaf.nimbus.NimbusLookAndFeel");
			UIManager
					.setLookAndFeel("javax.swing.plaf.nimbus.NimbusLookAndFeel");
		} catch (ClassNotFoundException e) {
			System.out
					.println("ClassNotFoundException: NimbusLookAndFeel not found");
		} catch (InstantiationException | IllegalAccessException
				| UnsupportedLookAndFeelException e) {
			e.printStackTrace();
		}
	}

	private void showGUI() {

		// PaintPanel erstellen und in das Fenster legen
		JMalPanel panel = new JMalPanel();

		inputTestData(panel);

		panel.setPreferredSize(new Dimension(400, 300));
		getContentPane().add(panel);

		// Fenster zentrieren und anzeigen
		pack();
		setLocationRelativeTo(null);
		setVisible(true);
	}

	private void inputTestData(JMalPanel panel) {
		double[] data0 = { 0, 40, 50, 70, 1 };
		double[] data1 = { 50, 70, 150, 80, 0 };
		double[] data2 = { 150, 80, 120, 170, 0 };
		double[] data3 = { 150, 80, 230, 20, 1 };
		double[] data4 = { 120, 170, 0, 190, 1 };
		double[] data5 = { 120, 170, 110, 300, 1 };
		double[] data6 = { 50, 70, 0, 100, 1 };
		double[] data7 = { 20, 40, 30, 70, 1 };

		panel.addPaintable(new VoronoiEdge(data0));
		panel.addPaintable(new VoronoiEdge(data1));
		panel.addPaintable(new VoronoiEdge(data2));
		panel.addPaintable(new VoronoiEdge(data3));
		panel.addPaintable(new VoronoiEdge(data4));
		panel.addPaintable(new VoronoiEdge(data5));
		panel.addPaintable(new VoronoiEdge(data6));
		panel.addPaintable(new VoronoiEdge(data7));
	}
}