import javax.swing.*;
import javax.swing.border.AbstractBorder;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.RoundRectangle2D;

/**
 * Small collection of reusable, custom-painted Swing components used to
 * give the MiniLang Compiler UI a more polished, modern look.
 *
 *  - RoundedButton     : a flat button with rounded corners and a hover/press glow
 *  - GradientPanel     : a JPanel that paints a soft diagonal gradient background
 *  - RoundedLineBorder : a rounded-corner replacement for BorderFactory.createLineBorder
 */
final class UIComponents {

    private UIComponents() {
    }

    /* ---------------------------------------------------------- */
    /*  Rounded, hover-aware button                                 */
    /* ---------------------------------------------------------- */
    static class RoundedButton extends JButton {

        private final Color baseColor;
        private final Color hoverColor;
        private final Color pressColor;
        private final int arc;

        private boolean hovering = false;
        private boolean pressing = false;

        RoundedButton(String text, Color baseColor) {
            super(text);

            this.baseColor = baseColor;
            this.hoverColor = brighten(baseColor, 0.16f);
            this.pressColor = brighten(baseColor, -0.14f);
            this.arc = 16;

            setContentAreaFilled(false);
            setFocusPainted(false);
            setBorderPainted(false);
            setOpaque(false);
            setForeground(Color.WHITE);
            setFont(new Font("Segoe UI", Font.BOLD, 14));
            setCursor(new Cursor(Cursor.HAND_CURSOR));
            setBorder(BorderFactory.createEmptyBorder(10, 24, 10, 24));
            setPreferredSize(new Dimension(170, 42));

            addMouseListener(new MouseAdapter() {
                @Override
                public void mouseEntered(MouseEvent e) {
                    hovering = true;
                    repaint();
                }

                @Override
                public void mouseExited(MouseEvent e) {
                    hovering = false;
                    repaint();
                }

                @Override
                public void mousePressed(MouseEvent e) {
                    pressing = true;
                    repaint();
                }

                @Override
                public void mouseReleased(MouseEvent e) {
                    pressing = false;
                    repaint();
                }
            });
        }

        private static Color brighten(Color color, float amount) {
            float[] hsb = Color.RGBtoHSB(
                    color.getRed(), color.getGreen(), color.getBlue(), null
            );

            float brightness = Math.max(0f, Math.min(1f, hsb[2] + amount));

            return Color.getHSBColor(hsb[0], hsb[1], brightness);
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(
                    RenderingHints.KEY_ANTIALIASING,
                    RenderingHints.VALUE_ANTIALIAS_ON
            );

            Color fill = !isEnabled()
                    ? new Color(70, 74, 88)
                    : pressing ? pressColor
                    : hovering ? hoverColor
                    : baseColor;

            g2.setColor(fill);
            g2.fillRoundRect(0, 0, getWidth(), getHeight(), arc, arc);

            if (hovering && isEnabled()) {
                g2.setColor(new Color(255, 255, 255, 35));
                g2.fillRoundRect(0, 0, getWidth(), getHeight() / 2, arc, arc);
            }

            g2.dispose();
            super.paintComponent(g);
        }

        @Override
        public boolean contains(int x, int y) {
            return new RoundRectangle2D.Float(0, 0, getWidth(), getHeight(), arc, arc)
                    .contains(x, y);
        }
    }

    /* ---------------------------------------------------------- */
    /*  Panel that paints a soft diagonal gradient background       */
    /* ---------------------------------------------------------- */
    static class GradientPanel extends JPanel {

        private final Color colorStart;
        private final Color colorEnd;

        GradientPanel(Color colorStart, Color colorEnd) {
            this.colorStart = colorStart;
            this.colorEnd = colorEnd;
            setOpaque(false);
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(
                    RenderingHints.KEY_ANTIALIASING,
                    RenderingHints.VALUE_ANTIALIAS_ON
            );

            GradientPaint gradient = new GradientPaint(
                    0, 0, colorStart,
                    getWidth(), getHeight(), colorEnd
            );

            g2.setPaint(gradient);
            g2.fillRect(0, 0, getWidth(), getHeight());
            g2.dispose();

            super.paintComponent(g);
        }
    }

    /* ---------------------------------------------------------- */
    /*  Rounded-corner replacement for a plain line border          */
    /* ---------------------------------------------------------- */
    static class RoundedLineBorder extends AbstractBorder {

        private final Color color;
        private final int thickness;
        private final int arc;

        RoundedLineBorder(Color color, int thickness, int arc) {
            this.color = color;
            this.thickness = thickness;
            this.arc = arc;
        }

        @Override
        public void paintBorder(
                Component c, Graphics g, int x, int y, int width, int height
        ) {
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(
                    RenderingHints.KEY_ANTIALIASING,
                    RenderingHints.VALUE_ANTIALIAS_ON
            );

            g2.setColor(color);
            g2.setStroke(new BasicStroke(thickness));
            g2.drawRoundRect(
                    x + thickness / 2, y + thickness / 2,
                    width - thickness - 1, height - thickness - 1,
                    arc, arc
            );

            g2.dispose();
        }

        @Override
        public Insets getBorderInsets(Component c) {
            return new Insets(thickness + 6, thickness + 8, thickness + 6, thickness + 8);
        }

        @Override
        public Insets getBorderInsets(Component c, Insets insets) {
            insets.left = insets.right = thickness + 8;
            insets.top = insets.bottom = thickness + 6;
            return insets;
        }

        @Override
        public boolean isBorderOpaque() {
            return false;
        }
    }
}