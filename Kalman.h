// kalman.h

#ifndef _KALMAN_h
#define _KALMAN_h

// Using a class here because some sensors are noisier than others, 
// and the Kalman filter depends on each sensor's value to work.
class Kalman {

public:
    // Constructor
    Kalman(double sensor_noise,double covariance,double environmental_uncertainty,double time_step) {
		this->R = sensor_noise;					// Data noise from the sensor
		this->Q = environmental_uncertainty;	// Uncertainty due to the environment
		this->x_k_1 = 0;						// Initial altitude
		this->v_k_1 = 0;						// Initial velocity
		this->P_k_1 = covariance;				// Initial covariance
		this->dt = time_step;					// Time step
    }
	
	Kalman(double sensor_noise,double covariance) {
		this->R_a = sensor_noise;
		this->a_k_1 = -9.80665;
		this->P_k_1_a = covariance;
	}

    // Use this method when we want to get the filtered output
    double * output_x(double z_k, double u_k,double a_k) {
		update_x(z_k,u_k,a_k);
		double x[2] = {x_t,v_t};
		return x;
    }
	
	double output_a(double a_k) {
		update_a(a_k);
		return a_t;
	}

private:

    double K_k;         // Kalman gain
    double P_k;         // Error covariance
    double P_k_1;    	// Error covariance from the previous iteration
	double P_t;         // Combined covariance
    double R;           // Covariance, or noise
	double Q;			// Environmental uncertainty
    double x_k;         // Altitude calculation
    double x_k_1;       // Altitude output from the last iteration
	double x_t;         // Combined altitude
	double v_k;         // Velocity calculation
    double v_k_1;       // Velocity output from the last iteration
	double v_t;		    // Combined velocity
    unsigned int k;     // Number of iterations
	double dt;			// Change in time
	
	double a_k_1;		// Previous acceleration
	double a_t;			// Combined acceleration
	double R_a;         // Covariance, or noise
	double P_k_1_a;    	// Error covariance from the previous iteration
	double P_t_a;       // Combined covariance
	double K_k_a;         // Kalman gain

    // Private method that saves the current variables to be used in the next iteration.
    void update_x(double z_k, double u_k,double a_k) {
		x_k = x_k_1 + (v_k_1*dt) + (0.5*a_k*(dt)^2);	// Calculate the new altitude
		v_k = v_k_1 + (a_k*dt);					  		// Calculate the new velocity
		P_k = P_k_1 + Q;						  		// Compute the new covariance
		K_k = P_k / (P_k + R);                    		// Compute the Kalman gain for this iteration
		x_t = x_k + (K_k * (z_k - x_k));		  		// Calculate the new filtered altitude output
		v_t = v_k + (K_k * (u_k - v_k));		  		// Calculate the new filtered velocity output
		P_t = (1 - K_k) * P_k;                  		// Calculate new error covariance
		P_k_1 = P_t;                              		// Set error covariance for next iteration
		x_k_1 = x_t;                              		// Set the old output for the next iteration
		k += 1;
    }
	
	void update_a(double a_k) {
		K_k_a = P_k_1_a / (P_k_1_a + R_a);          // Compute the Kalman gain for this iteration.
		a_t = a_k_1 + (K_k_a * (a_k - a_k_1));      // Calculate the new filtered output
		P_t_a = (1 - K_k_a) * P_k_1_a;              // Calculate new error covariance
		P_k_1_a = P_t_a;                            // Set error covariance for next iteration
		a_k_1 = a_t;                                // Set the old output for the next iteration
	}

};

#endif