// Uday Manchanda cs435 5402 pagerank

import java.io.*;
import java.util.*;
import java.text.DecimalFormat;

public class pagerank_5402 {

    private static double d = 0.85;
    private static int edges;
    private static int n;
    private static int iterations;
    private static int initialvalue;
    private static ArrayList<ArrayList<Integer>> adjacencylist;
    private static double[] currentpagerank;
    private static double[] oldpagerank;
    private static double[] outdegree;
    private static DecimalFormat df = new DecimalFormat("0.000000");
    private static double errorrate;
    private static double pumpedup;


    public static void main(String[] args) throws IOException {

        if (args.length != 3) {
            System.out.println("Incorrect parameter format. ");
            return;
        }

        iterations = Integer.parseInt(args[0]);
        initialvalue = Integer.parseInt(args[1]);
        String filename = args[2];

        File file = new File(filename);
        BufferedReader br = new BufferedReader(new FileReader(file));

        if (!(initialvalue >= -2 || initialvalue <= 2)) {
            System.out.println("invalid initial value");
            return;
        }

        String line1 = br.readLine();
        String[] ev = line1.split(" ");
        n = Integer.parseInt(ev[0]);
        edges = Integer.parseInt(ev[1]);

        if (n < 0 || edges < 0) {
            System.out.println("invalid number of vertices or edges");
            return;
        }

        pumpedup = (1-d)/n;
        double init = 0.0;

        switch(initialvalue) {
            case 0:
                init = 0;
                break;
            case 1:
                init = 1.0;
                break;
            case -1:
                init = (double)1/n;
                break;
            case -2:
                init = (double)1 / Math.sqrt(n);
                break;
            default:
                init = initialvalue;
                break;
        }
        
        
        // initialize graph as an adjacency list 
        adjacencylist = new ArrayList<ArrayList<Integer>>(n);
        currentpagerank = new double[n];
        oldpagerank = new double[n];
        outdegree = new double[n];

        for (int i = 0; i < n; i++) {
            currentpagerank[i] = init;
            oldpagerank[i] = init;
            adjacencylist.add(new ArrayList<>());
        }
    
        String line;
        while ((line = br.readLine()) != null) {
            String numbers[] = line.split(" ");
            int x = Integer.parseInt(numbers[0]);
            int y = Integer.parseInt(numbers[1]);
            if (x > n || y > n) {
                System.out.println("invalid values");
                break;
            }
            adjacencylist.get(x).add(y);
        }

        br.close();

        
        // initialize outdegree
        for (int i = 0; i < n; i++) {
            outdegree[i] = adjacencylist.get(i).size();
        }

        if (n > 10) {
            iterations = 0;
            initialvalue = -1;
        }

        int curriteration = 0;
        
        if (iterations > 0) {
            do {
                calculatepagerank();
                if (n > 0 && n < 10) {
                    printresults(curriteration);
                } else {
                    printresultsformorenodes(curriteration);
                }
                curriteration++;
            } while (curriteration <= iterations);
        } 

        else if (iterations == 0) {
            errorrate = Math.pow(10, -4);

            do {
                calculatepagerank();
                if (n > 0 && n < 10) {
                    printresults(curriteration);
                } else {
                    printresultsformorenodes(curriteration);
                }
                curriteration++;
            } while (!checkConvergence(errorrate));


        } else if (iterations < 0) {
            errorrate = Math.pow(10, iterations);

            do {
                calculatepagerank();
                if (n > 0 && n < 10) {
                    printresults(curriteration);
                } else {
                    printresultsformorenodes(curriteration);
                }
                curriteration++;
            } while (!checkConvergence(errorrate));
        }
    }

    public static void calculatepagerank() {
        double temppagerank[] = new double[n];
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (adjacencylist.get(j).contains(i)) {
                        temppagerank[i] += currentpagerank[j] / outdegree[j];
                    }
                }
                temppagerank[i] = pumpedup + d * temppagerank[i];
            }
        oldpagerank = currentpagerank;
        currentpagerank = temppagerank;
    }

    public static void printresults(int curriteration) {
        if (curriteration == 0) {
            System.out.print("Base   :  " + 0 + " :");
        } else {
            System.out.print("Iter   :  " + curriteration + " :");
        }
        
        for (int a = 0; a < n; a++) {
            System.out.print("P[ " + a + "]=" + df.format(currentpagerank[a]) + " ");
        }
        System.out.println();
    }

    public static void printresultsformorenodes(int curriteration) {
        System.out.println("Iter: " + curriteration);
        for (int a = 0; a < currentpagerank.length; a++) {
            System.out.println("P[" + a + "]=" + df.format(currentpagerank[a]));
        }
        System.out.println();
    }

    public static boolean checkConvergence(double errorrate) {
        for (int i = 0; i < n; i++) {
            if (Math.abs(currentpagerank[i] - oldpagerank[i]) > errorrate) {
                return false;
            }
        }
        return true;
    }
}