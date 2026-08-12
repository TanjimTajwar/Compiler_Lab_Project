import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class CompilerUI extends JFrame {

    private final JTextArea codeEditor;
    private final JTextArea compilerOutputArea;
    private final JTextArea tacArea;
    private final JTextArea assemblyArea;

    private final JButton browseButton;
    private final JButton compileButton;
    private final JButton clearButton;

    private final JLabel statusLabel;

    private final Color mutedColor = new Color(156, 163, 175);
    private final Color warningColor = new Color(250, 204, 21);
    private final Color successStatusColor = new Color(74, 222, 128);
    private final Color errorStatusColor = new Color(248, 113, 113);

    /*
     * This program is expected to run from:
     *
     * MiniCompiler/ui-java
     *
     * Therefore, the compiler project root is the parent directory.
     */
    private final Path uiDirectory;
    private final Path projectDirectory;
    private final Path compilerPath;
    private final Path temporarySourcePath;
    private final Path tacOutputPath;
    private final Path assemblyOutputPath;

    public CompilerUI() {

        /*
         * Resolve all paths once when the application starts.
         */
        uiDirectory = Path.of("").toAbsolutePath().normalize();
        projectDirectory = uiDirectory.getParent();

        if (projectDirectory == null) {
            throw new IllegalStateException(
                    "Unable to locate the MiniCompiler project directory."
            );
        }

        compilerPath = projectDirectory.resolve("minicompiler.exe");
        temporarySourcePath = projectDirectory.resolve("temp.ml");
        tacOutputPath = projectDirectory.resolve("output.tac");
        assemblyOutputPath = projectDirectory.resolve("output.asm");

        /*
         * Window configuration
         */
        setTitle("MiniLang Compiler");
        setSize(1400, 900);
        setMinimumSize(new Dimension(1000, 700));
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        /*
         * Application colors
         */
        Color backgroundColor = new Color(15, 17, 26);
        Color panelColor = new Color(26, 28, 40);
        Color borderColor = new Color(55, 65, 81);
        Color accentColor = new Color(34, 211, 238);
        Color successColor = new Color(74, 222, 128);
        Color sourceTextColor = new Color(243, 244, 246);
        Color assemblyTextColor = new Color(251, 191, 36);

        /*
         * Main container
         */
        JPanel mainPanel = new JPanel(new BorderLayout(12, 12));
        mainPanel.setBackground(backgroundColor);
        mainPanel.setBorder(new EmptyBorder(16, 20, 16, 20));

        /*
         * Header
         */
        JPanel headerPanel = new JPanel(new BorderLayout());
        headerPanel.setBackground(backgroundColor);
        headerPanel.setBorder(new EmptyBorder(0, 0, 12, 0));

        JPanel titlesPanel = new JPanel();
        titlesPanel.setLayout(new BoxLayout(titlesPanel, BoxLayout.Y_AXIS));
        titlesPanel.setOpaque(false);

        JLabel titleLabel = new JLabel(
                "MiniLang Compiler",
                SwingConstants.CENTER
        );

        titleLabel.setFont(new Font("Segoe UI", Font.BOLD, 32));
        titleLabel.setForeground(accentColor);
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel subtitleLabel = new JLabel(
                "Flex and Bison Compiler Interface",
                SwingConstants.CENTER
        );

        subtitleLabel.setFont(new Font("Segoe UI", Font.PLAIN, 14));
        subtitleLabel.setForeground(mutedColor);
        subtitleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        /*
         * Thin gradient accent bar beneath the title, purely decorative.
         */
        UIComponents.GradientPanel accentBar =
                new UIComponents.GradientPanel(accentColor, backgroundColor);
        accentBar.setPreferredSize(new Dimension(260, 3));
        accentBar.setMaximumSize(new Dimension(260, 3));
        accentBar.setAlignmentX(Component.CENTER_ALIGNMENT);

        titlesPanel.add(titleLabel);
        titlesPanel.add(Box.createRigidArea(new Dimension(0, 6)));
        titlesPanel.add(subtitleLabel);
        titlesPanel.add(Box.createRigidArea(new Dimension(0, 10)));
        titlesPanel.add(accentBar);

        headerPanel.add(titlesPanel, BorderLayout.CENTER);

        /*
         * Status pill in the top-right corner, updated as compilation
         * moves from idle to running to finished.
         */
        statusLabel = new JLabel();
        statusLabel.setFont(new Font("Segoe UI", Font.BOLD, 13));

        JPanel statusWrapper = new JPanel(new GridBagLayout());
        statusWrapper.setOpaque(false);
        statusWrapper.add(statusLabel);

        headerPanel.add(statusWrapper, BorderLayout.EAST);

        mainPanel.add(headerPanel, BorderLayout.NORTH);

        /*
         * Source-code editor
         */
        codeEditor = createTextArea(
                panelColor,
                sourceTextColor,
                true
        );

        codeEditor.setText(
                "int arnab_counter;\n" +
                "int jami_sum;\n" +
                "bool muznabin_condition;\n\n" +
                "arnab_counter = 0;\n" +
                "jami_sum = 0;\n\n" +
                "while (arnab_counter < 5)\n" +
                "{\n" +
                "    jami_sum = jami_sum + arnab_counter;\n" +
                "    arnab_counter = arnab_counter + 1;\n" +
                "}\n\n" +
                "muznabin_condition = jami_sum == 10;\n\n" +
                "if (muznabin_condition)\n" +
                "{\n" +
                "    print(jami_sum);\n" +
                "}\n" +
                "else\n" +
                "{\n" +
                "    print(0);\n" +
                "}\n"
        );

        JPanel sourcePanel = createOutputPanel(
                "Source Code",
                codeEditor,
                backgroundColor,
                borderColor
        );

        /*
         * Compiler output
         */
        compilerOutputArea = createTextArea(
                panelColor,
                successColor,
                false
        );

        compilerOutputArea.setText(
                "Compiler output, AST, semantic results, and errors " +
                "will appear here.\n"
        );

        JPanel compilerOutputPanel = createOutputPanel(
                "Compiler Output",
                compilerOutputArea,
                backgroundColor,
                borderColor
        );

        /*
         * TAC output
         */
        tacArea = createTextArea(
                panelColor,
                accentColor,
                false
        );

        tacArea.setText(
                "The contents of output.tac will appear here " +
                "after successful compilation.\n"
        );

        JPanel tacPanel = createOutputPanel(
                "Three-Address Code (output.tac)",
                tacArea,
                backgroundColor,
                borderColor
        );

        /*
         * Assembly output
         */
        assemblyArea = createTextArea(
                panelColor,
                assemblyTextColor,
                false
        );

        assemblyArea.setText(
                "The contents of output.asm will appear here " +
                "after successful compilation.\n"
        );

        JPanel assemblyPanel = createOutputPanel(
                "Pseudo Assembly (output.asm)",
                assemblyArea,
                backgroundColor,
                borderColor
        );

        /*
         * Right side contains separate TAC and Assembly boxes.
         */
        JSplitPane tacAssemblySplitPane = new JSplitPane(
                JSplitPane.VERTICAL_SPLIT,
                tacPanel,
                assemblyPanel
        );

        tacAssemblySplitPane.setResizeWeight(0.5);
        tacAssemblySplitPane.setDividerLocation(330);
        tacAssemblySplitPane.setDividerSize(10);
        tacAssemblySplitPane.setBorder(null);
        tacAssemblySplitPane.setBackground(backgroundColor);

        /*
         * Lower half contains compiler output on the left
         * and TAC/Assembly on the right.
         */
        JSplitPane resultSplitPane = new JSplitPane(
                JSplitPane.HORIZONTAL_SPLIT,
                compilerOutputPanel,
                tacAssemblySplitPane
        );

        resultSplitPane.setResizeWeight(0.52);
        resultSplitPane.setDividerLocation(680);
        resultSplitPane.setDividerSize(10);
        resultSplitPane.setBorder(null);
        resultSplitPane.setBackground(backgroundColor);

        /*
         * Main vertical split:
         * source editor on top, all outputs below.
         */
        JSplitPane mainSplitPane = new JSplitPane(
                JSplitPane.VERTICAL_SPLIT,
                sourcePanel,
                resultSplitPane
        );

        mainSplitPane.setResizeWeight(0.46);
        mainSplitPane.setDividerLocation(350);
        mainSplitPane.setDividerSize(10);
        mainSplitPane.setBorder(null);
        mainSplitPane.setBackground(backgroundColor);

        mainPanel.add(mainSplitPane, BorderLayout.CENTER);

        /*
         * Buttons
         */
        JPanel buttonPanel = new JPanel(
                new FlowLayout(FlowLayout.CENTER, 16, 6)
        );

        buttonPanel.setBackground(backgroundColor);
        buttonPanel.setBorder(new EmptyBorder(12, 0, 0, 0));

        browseButton = createButton(
                "Browse .ml File",
                new Color(59, 130, 246),
                Color.WHITE
        );

        compileButton = createButton(
                "Compile",
                new Color(16, 185, 129),
                Color.WHITE
        );

        clearButton = createButton(
                "Clear",
                new Color(239, 68, 68),
                Color.WHITE
        );

        buttonPanel.add(browseButton);
        buttonPanel.add(compileButton);
        buttonPanel.add(clearButton);

        mainPanel.add(buttonPanel, BorderLayout.SOUTH);

        /*
         * Button actions
         */
        browseButton.addActionListener(event -> browseSourceFile());

        compileButton.addActionListener(event -> compileCode());

        clearButton.addActionListener(event -> clearAll());

        setStatus("Ready", mutedColor);

        setContentPane(mainPanel);
        setVisible(true);
    }

    /*
     * Update the status pill shown in the top-right corner of the header.
     */
    private void setStatus(String text, Color color) {
        statusLabel.setText("\u25CF  " + text);
        statusLabel.setForeground(color);
    }

    /*
     * Create a consistently styled text area.
     */
    private JTextArea createTextArea(
            Color background,
            Color foreground,
            boolean editable
    ) {

        JTextArea textArea = new JTextArea();

        textArea.setEditable(editable);
        textArea.setFont(new Font("Consolas", Font.PLAIN, 15));
        textArea.setBackground(background);
        textArea.setForeground(foreground);
        textArea.setCaretColor(Color.WHITE);
        textArea.setLineWrap(false);
        textArea.setTabSize(4);
        textArea.setMargin(new Insets(10, 12, 10, 12));

        return textArea;
    }

    /*
     * Create a titled, rounded "card" panel containing a scrollable text
     * area, used for the source editor and each of the output panes.
     */
    private JPanel createOutputPanel(
            String title,
            JTextArea textArea,
            Color background,
            Color borderColor
    ) {

        JPanel panel = new JPanel(new BorderLayout(0, 8));
        panel.setBackground(background);
        panel.setBorder(BorderFactory.createCompoundBorder(
                new UIComponents.RoundedLineBorder(borderColor, 1, 14),
                new EmptyBorder(6, 8, 8, 8)
        ));

        JLabel titleLabel = new JLabel(
                "<html><span style='color:#6B7280;'>\u25CF</span>"
                + "&nbsp;&nbsp;" + title + "</html>"
        );

        titleLabel.setFont(new Font("Segoe UI", Font.BOLD, 14));
        titleLabel.setForeground(Color.WHITE);
        titleLabel.setBorder(new EmptyBorder(2, 4, 2, 4));

        JScrollPane scrollPane = new JScrollPane(textArea);

        scrollPane.setBorder(BorderFactory.createEmptyBorder());
        scrollPane.getVerticalScrollBar().setUnitIncrement(16);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(scrollPane, BorderLayout.CENTER);

        return panel;
    }

    /*
     * Create a styled, rounded, hover-aware button.
     */
    private JButton createButton(
            String text,
            Color background,
            Color foreground
    ) {

        UIComponents.RoundedButton button =
                new UIComponents.RoundedButton(text, background);

        button.setForeground(foreground);

        return button;
    }

    /*
     * Browse and load a MiniLang source file.
     */
    private void browseSourceFile() {

        JFileChooser chooser = new JFileChooser(
                projectDirectory.resolve("testcases").toFile()
        );

        chooser.setDialogTitle("Select a MiniLang source file");

        chooser.setFileFilter(
                new FileNameExtensionFilter(
                        "MiniLang source files (*.ml)",
                        "ml"
                )
        );

        int result = chooser.showOpenDialog(this);

        if (result != JFileChooser.APPROVE_OPTION) {
            return;
        }

        File selectedFile = chooser.getSelectedFile();

        try {

            String sourceCode = Files.readString(
                    selectedFile.toPath(),
                    StandardCharsets.UTF_8
            );

            codeEditor.setText(sourceCode);
            codeEditor.setCaretPosition(0);

            compilerOutputArea.setText(
                    "Loaded source file successfully:\n\n"
                    + selectedFile.getAbsolutePath()
            );

            tacArea.setText("");
            assemblyArea.setText("");

            setStatus("File loaded", mutedColor);

        } catch (IOException exception) {

            showError(
                    "Unable to read the selected file:\n"
                    + exception.getMessage()
            );
        }
    }

    /*
     * Clear the editor and all output panels.
     */
    private void clearAll() {

        codeEditor.setText("");
        compilerOutputArea.setText("");
        tacArea.setText("");
        assemblyArea.setText("");

        setStatus("Ready", mutedColor);

        codeEditor.requestFocusInWindow();
    }

    /*
     * Compile the source code without freezing the GUI.
     */
    private void compileCode() {

        String sourceCode = codeEditor.getText();

        if (sourceCode.trim().isEmpty()) {

            showError(
                    "The source-code editor is empty.\n" +
                    "Write MiniLang code or browse an .ml file first."
            );

            return;
        }

        if (!Files.exists(compilerPath)) {

            showError(
                    "Compiler executable was not found:\n\n"
                    + compilerPath
                    + "\n\nRun 'make' from the MiniCompiler project folder first."
            );

            return;
        }

        compileButton.setEnabled(false);
        browseButton.setEnabled(false);

        setStatus("Compiling\u2026", warningColor);

        compilerOutputArea.setForeground(
                new Color(250, 204, 21)
        );

        compilerOutputArea.setText("Compiling MiniLang source code...\n");
        tacArea.setText("");
        assemblyArea.setText("");

        SwingWorker<CompilationResult, Void> worker =
                new SwingWorker<>() {

                    @Override
                    protected CompilationResult doInBackground()
                            throws Exception {

                        /*
                         * Remove old outputs first so an unsuccessful
                         * compilation cannot display stale TAC or ASM.
                         */
                        Files.deleteIfExists(tacOutputPath);
                        Files.deleteIfExists(assemblyOutputPath);

                        Files.writeString(
                                temporarySourcePath,
                                sourceCode,
                                StandardCharsets.UTF_8
                        );

                        ProcessBuilder processBuilder =
                                new ProcessBuilder(
                                        compilerPath.toString(),
                                        temporarySourcePath.toString()
                                );

                        /*
                         * Run from the project directory because the C
                         * compiler writes output.tac and output.asm there.
                         */
                        processBuilder.directory(
                                projectDirectory.toFile()
                        );

                        processBuilder.redirectErrorStream(true);

                        Process process = processBuilder.start();

                        StringBuilder compilerOutput =
                                new StringBuilder();

                        try (
                                BufferedReader reader =
                                        new BufferedReader(
                                                new InputStreamReader(
                                                        process.getInputStream(),
                                                        StandardCharsets.UTF_8
                                                )
                                        )
                        ) {

                            String line;

                            while ((line = reader.readLine()) != null) {

                                compilerOutput
                                        .append(line)
                                        .append(System.lineSeparator());
                            }
                        }

                        int exitCode = process.waitFor();

                        String tacContent = readOutputFile(tacOutputPath);
                        String assemblyContent =
                                readOutputFile(assemblyOutputPath);

                        return new CompilationResult(
                                exitCode,
                                compilerOutput.toString(),
                                tacContent,
                                assemblyContent
                        );
                    }

                    @Override
                    protected void done() {

                        compileButton.setEnabled(true);
                        browseButton.setEnabled(true);

                        try {

                            CompilationResult result = get();

                            compilerOutputArea.setText(
                                    result.compilerOutput
                            );

                            compilerOutputArea.setCaretPosition(0);

                            if (result.exitCode == 0) {

                                compilerOutputArea.setForeground(
                                        new Color(74, 222, 128)
                                );

                                setStatus("Success", successStatusColor);

                            } else {

                                compilerOutputArea.setForeground(
                                        new Color(248, 113, 113)
                                );

                                setStatus("Failed", errorStatusColor);
                            }

                            if (result.tacContent.isBlank()) {

                                tacArea.setText(
                                        "No TAC output was generated.\n\n" +
                                        "Check the compiler output for " +
                                        "lexical, syntax, or semantic errors."
                                );

                            } else {

                                tacArea.setText(result.tacContent);
                            }

                            if (result.assemblyContent.isBlank()) {

                                assemblyArea.setText(
                                        "No assembly output was generated.\n\n" +
                                        "Check the compiler output for errors."
                                );

                            } else {

                                assemblyArea.setText(
                                        result.assemblyContent
                                );
                            }

                            tacArea.setCaretPosition(0);
                            assemblyArea.setCaretPosition(0);

                        } catch (Exception exception) {

                            showError(
                                    "Compilation failed:\n"
                                    + getUsefulErrorMessage(exception)
                            );
                        }
                    }
                };

        worker.execute();
    }

    /*
     * Read TAC or Assembly if the file exists.
     */
    private String readOutputFile(Path outputPath)
            throws IOException {

        if (!Files.exists(outputPath)) {
            return "";
        }

        return Files.readString(
                outputPath,
                StandardCharsets.UTF_8
        );
    }

    /*
     * Display an error in the compiler output panel.
     */
    private void showError(String message) {

        compilerOutputArea.setForeground(
                new Color(248, 113, 113)
        );

        compilerOutputArea.setText(
                "ERROR\n\n" + message
        );

        compilerOutputArea.setCaretPosition(0);

        setStatus("Error", errorStatusColor);
    }

    /*
     * Extract a useful message from nested SwingWorker exceptions.
     */
    private String getUsefulErrorMessage(Throwable throwable) {

        Throwable current = throwable;

        while (
                current.getCause() != null
                && current.getCause() != current
        ) {
            current = current.getCause();
        }

        String message = current.getMessage();

        if (message == null || message.isBlank()) {
            return current.toString();
        }

        return message;
    }

    /*
     * Store the result returned from the background compilation task.
     */
    private static class CompilationResult {

        private final int exitCode;
        private final String compilerOutput;
        private final String tacContent;
        private final String assemblyContent;

        private CompilationResult(
                int exitCode,
                String compilerOutput,
                String tacContent,
                String assemblyContent
        ) {

            this.exitCode = exitCode;
            this.compilerOutput = compilerOutput;
            this.tacContent = tacContent;
            this.assemblyContent = assemblyContent;
        }
    }
}
