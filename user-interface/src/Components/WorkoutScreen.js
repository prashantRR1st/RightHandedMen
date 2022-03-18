import React from "react";
import Header from "./Header";
import WorkoutChart from "./WorkoutChart";
import { Link } from "react-router-dom";
import Button from '@mui/material/Button';
import Typography from '@mui/material/Typography';

export default function WorkoutScreen() {
    return(
        <div style={{backgroundColor: '#121212', height: '100%'}}>
            <Header></Header>
            <Typography gutterBottom variant="h4" component="div" style={{textAlign: 'center', color: '#ffffff'}}>
                Active Workout
            </Typography>
            <WorkoutChart></WorkoutChart>
            <Link to="/summary" style={{padding: 100}}>
                <Button variant="contained" size="large">
                    <span>End Workout</span>
                </Button>
            </Link>
        </div>
    );
};
