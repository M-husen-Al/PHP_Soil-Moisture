<?php
// Konfigurasi koneksi database
$servername = "localhost";
$username = "root"; // Sesuaikan dengan username MySQL Anda
$password = ""; // Sesuaikan dengan password MySQL Anda
$dbname = "sensor_soilmoisture";

// Membuat koneksi
$conn = new mysqli($servername, $username, $password, $dbname);

// Periksa koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}

// Query untuk mengambil data dari tabel soilmoisture
$sql = "SELECT * FROM soilmoisture";
$result = $conn->query($sql);

// Periksa jika ada data yang dikirim via POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['Kelembaban_tanah'])) {
        $kelembaban_tanah = $_POST['Kelembaban_tanah'];

        // Query untuk menyimpan data ke database
        $sql_insert = "INSERT INTO soilmoisture (`Kelembaban tanah`) VALUES ('$kelembaban_tanah')";
        
        if ($conn->query($sql_insert) === TRUE) {
            echo "<div class='success-message'>Data berhasil disimpan!</div>";
        } else {
            echo "<div class='error-message'>Error: " . $sql_insert . "<br>" . $conn->error . "</div>";
        }
    } else {
        echo "<div class='error-message'>Parameter Kelembaban_tanah tidak ditemukan!</div>";
    }
}

?>

<!DOCTYPE html>
<html>
<head>
    <title>Data Kelembaban Tanah</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f9;
            margin: 0;
            padding: 0;
        }
        h2 {
            text-align: center;
            color: #333;
            margin-top: 20px;
        }
        table {
            width: 80%;
            margin: 20px auto;
            border-collapse: collapse;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
        }
        table, th, td {
            border: 1px solid #ddd;
        }
        th, td {
            padding: 12px;
            text-align: center;
        }
        th {
            background-color: #007BFF;
            color: white;
        }
        tr:nth-child(even) {
            background-color: #f2f2f2;
        }
        tr:hover {
            background-color: #e0e0e0;
        }
        .success-message {
            text-align: center;
            color: green;
            font-weight: bold;
            margin-top: 10px;
        }
        .error-message {
            text-align: center;
            color: red;
            font-weight: bold;
            margin-top: 10px;
        }
    </style>
</head>
<body>
    <h2>Data Kelembaban Tanah</h2>
    <table>
        <thead>
            <tr>
                <th>ID</th>
                <th>Kelembaban Tanah</th>
                <th>Datetime</th>
            </tr>
        </thead>
        <tbody>
            <?php
            if ($result->num_rows > 0) {
                // Menampilkan data dari setiap baris
                while ($row = $result->fetch_assoc()) {
                    echo "<tr>";
                    echo "<td>" . $row['id'] . "</td>";
                    echo "<td>" . $row['Kelembaban tanah'] . "</td>";
                    echo "<td>" . $row['datetime'] . "</td>";
                    echo "</tr>";
                }
            } else {
                echo "<tr><td colspan='3'>Tidak ada data</td></tr>";
            }
            ?>
        </tbody>
    </table>
</body>
</html>

<?php
// Menutup koneksi
$conn->close();
?>
