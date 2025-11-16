package com.example.cars.app.navigation

import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavHostController
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.navArgument
import com.example.cars.ui.auth.LoginScreen
import com.example.cars.ui.auth.LoginViewModel
import com.example.cars.ui.cars.detail.CarDetailScreen
import com.example.cars.ui.cars.detail.CarDetailViewModel
import com.example.cars.ui.cars.list.CarsListScreen
import com.example.cars.ui.cars.list.CarsListViewModel

sealed class Screen(val route: String) {
    object Login : Screen("login")
    object CarsList : Screen("cars_list")
    object CarDetail : Screen("car_detail/{carId}") {
        fun createRoute(carId: String = "new") = "car_detail/$carId"
    }
}

@Composable
fun NavGraph(
    navController: NavHostController,
    startDestination: String
) {
    NavHost(
        navController = navController,
        startDestination = startDestination
    ) {
        composable(Screen.Login.route) {
            val viewModel: LoginViewModel = hiltViewModel()
            LoginScreen(
                viewModel = viewModel,
                onLoginSuccess = {
                    navController.navigate(Screen.CarsList.route) {
                        popUpTo(Screen.Login.route) { inclusive = true }
                    }
                }
            )
        }

        composable(Screen.CarsList.route) {
            val viewModel: CarsListViewModel = hiltViewModel()
            CarsListScreen(
                viewModel = viewModel,
                onCarClick = { carId ->
                    navController.navigate(Screen.CarDetail.createRoute(carId))
                },
                onAddCarClick = {
                    navController.navigate(Screen.CarDetail.createRoute())
                },
                onLogout = {
                    navController.navigate(Screen.Login.route) {
                        popUpTo(0) { inclusive = true }
                    }
                }
            )
        }

        composable(
            route = Screen.CarDetail.route,
            arguments = listOf(
                navArgument("carId") {
                    type = NavType.StringType
                    defaultValue = "new"
                }
            )
        ) { backStackEntry ->
            val carId = backStackEntry.arguments?.getString("carId") ?: "new"
            val viewModel: CarDetailViewModel = hiltViewModel()
            
            CarDetailScreen(
                viewModel = viewModel,
                carId = if (carId == "new") null else carId,
                onNavigateBack = {
                    navController.popBackStack()
                }
            )
        }
    }
}

