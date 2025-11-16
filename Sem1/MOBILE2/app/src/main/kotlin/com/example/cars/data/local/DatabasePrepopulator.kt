package com.example.cars.data.local

import androidx.room.RoomDatabase
import androidx.sqlite.db.SupportSQLiteDatabase
import java.time.Instant

class DatabasePrepopulator : RoomDatabase.Callback() {

    override fun onCreate(db: SupportSQLiteDatabase) {
        super.onCreate(db)
        
        // Prepopulate database with sample cars using raw SQL
        val timestamp = Instant.now().toEpochMilli()
        
        val sampleCars = listOf(
            // Tesla Model 3
            arrayOf("1", "Tesla", "Model 3", 2023, 45990.0, 5420, "Electric", "Automatic",
                "https://images.unsplash.com/photo-1560958089-b8a1929cea89?w=800&q=80",
                "Premium electric sedan with autopilot, long range battery, and cutting-edge technology. Perfect condition with minimal mileage.",
                timestamp, 44.4268, 26.1025, 0, 0),
            
            // BMW M4
            arrayOf("2", "BMW", "M4 Competition", 2022, 78900.0, 12500, "Gasoline", "Automatic",
                "https://images.unsplash.com/photo-1617531653520-bd4f14c3e0a3?w=800&q=80",
                "High-performance coupe with twin-turbo engine, carbon fiber roof, and M Sport exhaust. Track-ready beast.",
                timestamp, 44.4350, 26.1028, 0, 0),
            
            // Mercedes-Benz C-Class
            arrayOf("3", "Mercedes-Benz", "C-Class", 2021, 42500.0, 28000, "Diesel", "Automatic",
                "https://images.unsplash.com/photo-1618843479313-40f8afb4b4d8?w=800&q=80",
                "Luxury sedan with premium leather interior, advanced safety features, and exceptional fuel efficiency.",
                timestamp, 44.4400, 26.0950, 0, 0),
            
            // Porsche 911
            arrayOf("4", "Porsche", "911 Carrera", 2023, 115000.0, 3200, "Gasoline", "Manual",
                "https://images.unsplash.com/photo-1614200187524-dc4b892acf16?w=800&q=80",
                "Iconic sports car with naturally aspirated flat-six engine. Manual transmission for the true driving enthusiast.",
                timestamp, 44.4200, 26.1100, 0, 0),
            
            // Audi e-tron GT
            arrayOf("5", "Audi", "e-tron GT", 2023, 102700.0, 8900, "Electric", "Automatic",
                "https://images.unsplash.com/photo-1606664515524-ed2f786a0bd6?w=800&q=80",
                "All-electric grand tourer with stunning design, Quattro all-wheel drive, and ultra-fast charging capability.",
                timestamp, 44.4320, 26.1080, 0, 0),
            
            // Ford Mustang GT
            arrayOf("6", "Ford", "Mustang GT", 2022, 38500.0, 15600, "Gasoline", "Manual",
                "https://images.unsplash.com/photo-1584345604476-8ec5f5e0b7a7?w=800&q=80",
                "American muscle car with V8 power, aggressive styling, and thrilling exhaust note. True driver's car.",
                timestamp, 44.4180, 26.0990, 0, 0),
            
            // Toyota RAV4
            arrayOf("7", "Toyota", "RAV4 Hybrid", 2023, 34200.0, 6800, "Hybrid", "Automatic",
                "https://images.unsplash.com/photo-1621007947382-bb3c3994e3fb?w=800&q=80",
                "Reliable hybrid SUV with excellent fuel economy, spacious interior, and Toyota's renowned reliability.",
                timestamp, 44.4450, 26.1150, 0, 0),
            
            // VW Golf GTI
            arrayOf("8", "Volkswagen", "Golf GTI", 2022, 32900.0, 19200, "Gasoline", "Manual",
                "https://images.unsplash.com/photo-1622016541378-e5c61e84fa77?w=800&q=80",
                "Hot hatch perfection with turbocharged engine, sport suspension, and iconic plaid seats. Fun to drive!",
                timestamp, 44.4290, 26.1200, 0, 0),
            
            // Lexus RX 350
            arrayOf("9", "Lexus", "RX 350", 2021, 48700.0, 22000, "Gasoline", "Automatic",
                "https://images.unsplash.com/photo-1616422285623-13ff0162193c?w=800&q=80",
                "Luxury SUV with whisper-quiet cabin, premium materials, and exceptional comfort for long journeys.",
                timestamp, 44.4380, 26.1010, 0, 0),
            
            // Honda Civic Type R
            arrayOf("10", "Honda", "Civic Type R", 2023, 44200.0, 4100, "Gasoline", "Manual",
                "https://images.unsplash.com/photo-1590362891991-f776e747a588?w=800&q=80",
                "Track-focused hot hatch with turbocharged VTEC engine, limited-slip differential, and race-inspired interior.",
                timestamp, 44.4150, 26.1120, 0, 0),
            
            // Chevrolet Corvette
            arrayOf("11", "Chevrolet", "Corvette C8", 2023, 68400.0, 2500, "Gasoline", "Automatic",
                "https://images.unsplash.com/photo-1611859266238-4b98091d9d9b?w=800&q=80",
                "Mid-engine American supercar with breathtaking acceleration, exotic looks, and surprisingly practical.",
                timestamp, 44.4420, 26.0920, 0, 0),
            
            // Mazda MX-5 Miata
            arrayOf("12", "Mazda", "MX-5 Miata", 2022, 28900.0, 11400, "Gasoline", "Manual",
                "https://images.unsplash.com/photo-1603386329225-868f9b1ee6b9?w=800&q=80",
                "Lightweight roadster with perfect handling balance, retractable soft-top, and pure driving joy.",
                timestamp, 44.4100, 26.1050, 0, 0),
            
            // Lamborghini Huracan
            arrayOf("13", "Lamborghini", "Huracán EVO", 2022, 261274.0, 1800, "Gasoline", "Automatic",
                "https://images.unsplash.com/photo-1621135802920-133df287f89c?w=800&q=80",
                "Exotic Italian supercar with V10 engine, all-wheel drive, and head-turning design. Ultimate dream car.",
                timestamp, 44.4480, 26.0880, 0, 0),
            
            // Jeep Wrangler
            arrayOf("14", "Jeep", "Wrangler Rubicon", 2023, 52900.0, 7200, "Gasoline", "Automatic",
                "https://images.unsplash.com/photo-1606220838315-056192d5e927?w=800&q=80",
                "Off-road champion with removable doors and roof, locking differentials, and go-anywhere capability.",
                timestamp, 44.4250, 26.0850, 0, 0),
            
            // Nissan GT-R
            arrayOf("15", "Nissan", "GT-R NISMO", 2022, 215800.0, 3900, "Gasoline", "Automatic",
                "https://images.unsplash.com/photo-1600705722908-bab1e61c0b4d?w=800&q=80",
                "Japanese supercar with twin-turbo V6, advanced AWD system, and legendary performance. Godzilla unleashed!",
                timestamp, 44.4500, 26.1180, 0, 0)
        )
        
        // Insert all sample cars
        db.beginTransaction()
        try {
            sampleCars.forEach { car ->
                db.execSQL(
                    """
                    INSERT INTO cars (id, make, model, year, price, mileage, fuelType, transmission, 
                                     imageUrl, description, updatedAt, lat, lng, isPendingSync, isDeleted)
                    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                    """.trimIndent(),
                    car
                )
            }
            db.setTransactionSuccessful()
        } finally {
            db.endTransaction()
        }
    }
}
