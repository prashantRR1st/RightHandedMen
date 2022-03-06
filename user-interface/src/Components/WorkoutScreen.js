import React from "react";
import Header from "./Header";
import WorkoutChart from "./WorkoutChart";
import { Link } from "react-router-dom";
import Button from '@mui/material/Button';

export default function WorkoutScreen() {
    return(
        <div>
            <Header></Header>
            <h1>Workout</h1>
            <WorkoutChart></WorkoutChart>
            <Link to="/summary">
                <Button variant="contained">
                    <span>End Workout</span>
                </Button>
            </Link>
        </div>
    );
};
