/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package calc;

import javax.ws.rs.core.Context;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PUT;
import javax.ws.rs.PathParam;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

/**
 * REST Web Service
 *
 * @author angel
 */
@Path("calc/{op}")
public class Calculator {

    @Context
    private UriInfo context;

    /**
     * Creates a new instance of Calculator
     */
    public Calculator() {
    }

    /**
     * Retrieves representation of an instance of calc.Calculator
     *
     * @param op
     * @param n1
     * @param n2
     * @return an instance of java.lang.String
     */
    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public Response getText(@PathParam("op") String op, @QueryParam("n1") double n1, @QueryParam("n2") double n2) {
        double res;
        switch (op) {
            case "plus":
                res = n1 + n2;
                break;
            case "minus":
                res = n1 - n2;
                break;
            case "mult":
                res = n1 * n2;
                break;
            case "div":
                res = n1 / n2;
                break;
            default:
                return Response.status(Response.Status.NOT_FOUND).build();
        }
        return Response.ok(Double.toString(res)).build();
    }

    /**
     * PUT method for updating or creating an instance of Calculator
     *
     * @param content representation for the resource
     */
    @PUT
    @Consumes(MediaType.TEXT_PLAIN)
    public void putText(String content) {
    }
}
