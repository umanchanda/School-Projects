// Uday Manchanda cs435 5402 hashing

import java.io.*;

class HashTable {

    public static int[] array;
    public static int size;
    public static int collisions;

    public HashTable(int s) {
        this.size = s;
        array = new int[s];
        initArray();
    }

    public void initArray() {
        for (int i = 0; i < size; i++) {
            this.array[i] = -1;
        }
    }

    public int getCount() {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (array[i] != -1) {
                count++;
            }
        }
        return count;
    }
}

class Lexicon {
    public static HashTable T;
    public static char[] A;

    public int getSize() {
        return T.size;
    }
}

public class hash_5402 {

    private static int inserted = 0;

    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.out.println("Invalid arguments supplied");
            return;
        }
        File file = new File(args[0]);
        Lexicon l = new Lexicon();
        HashBatch(l, file);
    }

    public static void HashBatch(Lexicon l, File filename) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line1 = br.readLine();
        String[] arguments = line1.split(" ");
        int x = Integer.parseInt(arguments[0]);
        if (x == 14) {
            int size = Integer.parseInt(arguments[1]);
            HashCreate(l, size);
        } else {
            System.out.println("Lexicon has not been initialized");
            return;
        }

        String line;
        while ((line = br.readLine()) != null) {
            String[] commands = line.split(" ");
            int operation = Integer.parseInt(commands[0]);
            if (operation == 13) {
                HashPrint(l);
                continue;
            }

            String word = commands[1];
            switch(operation) {
                case 10:
                    HashInsert(l, word);
                    break;
                case 11:
                    HashDelete(l, word);
                    break;
                case 12:
                    // HashSearch(l, word);
                    int index = HashSearch(l, word);
                    if (index != -1) {
                        System.out.println(word + "\tfound at slot " + index);
                    } else {
                        System.out.println(word + "\tnot found");
                    }
                    break;
                default:
                    continue;
            }
        }
        br.close();
    }

    public static void HashCreate(Lexicon l, int m) {
        l = new Lexicon();
        l.T = new HashTable(m);
        l.A = new char[m*15];
    }

    // returns true if empty, false otherwise
    public static boolean HashEmpty(Lexicon l) {
        return (l.T.getCount() == 0);
    }

    // returns true if full, false otherwise
    public static boolean HashFull(Lexicon l) {
        return (l.T.getCount() == l.getSize());
    }

    public static void HashPrint(Lexicon l) {
        System.out.println();
        System.out.print("\tT\t\tA: ");
        for (int i = 0; i < l.A.length; i++) {
            System.out.print(l.A[i]);
            if (l.A[i] == '\0' && i < inserted) {
                System.out.print("\\");
            }
        }
        System.out.println();

        for (int i = 0; i < l.T.array.length; i++) {
            System.out.print(i + ": ");
            if (l.T.array[i] != -1) {
                System.out.print(l.T.array[i]);
            }
            System.out.println();
        }
        System.out.println();
    }

    public static void HashInsert(Lexicon l, String w) {

        if (HashFull(l)) {
            System.out.println("Hash Table is full");
            return;
        }

        int insertion = findPosition(l, w);

        w = w + '\0';
        if (w.length() >= l.A.length) {
            l.A = resize(l.A);
        }

        l.T.array[insertion] = inserted;
        for (int i = 0; i < w.length(); i++) {
            l.A[inserted] = w.charAt(i);
            inserted++;
        }
    }

    public static void HashDelete(Lexicon l, String w) {
        if (HashEmpty(l)) {
            System.out.println("hash table is empty");
        }

        int index = HashSearch(l, w);
        if (index != -1) {
            int oldvalue = l.T.array[index];
            l.T.array[index] = -1;
            while (oldvalue < w.length()) {
                l.A[oldvalue] = '*';
                oldvalue++;
            }
            System.out.println(w + "\tdeleted from slot " + index);
        }

    }

    public static int HashSearch(Lexicon l, String w) {
        int index = findPosition(l, w);
        if (l.T.array[index] == -1 || index == -1) {
            return -1;
        }
        return index;
    }

    private static char[] resize(char[] arr) {
        char[] newarray = new char[arr.length * 2];
        for (int i = 0; i < arr.length; i++) {
            newarray[i] = arr[i];
        }
        return newarray;
    }

    public static int findPosition(Lexicon l, String w) {
        int h;
        int probing = 0;
        int sum = 0;

        for (Character c : w.toCharArray()) {
            sum += (int)c;
        }

        h = sum % l.getSize();

        if (l.T.array[h] != -1) {
            do {
                probing++;
                h = (h + (probing*probing)) % l.getSize();
            } while (probing < l.getSize() || l.T.array[h] != -1);
        }
        return h;
    }
}