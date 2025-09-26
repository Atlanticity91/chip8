require( "vstudio" )

---
-- Allow static library projects to link in their dependencies. Visual Studio
-- doesn't support this out of the box, so I need to manually add items to
-- the list of additional dependencies.
---

local p = premake

function myAdditionalStaticDependencies(cfg)
    local links = p.config.getlinks(cfg, "siblings", "fullpath")
    if #links > 0 then
        links = path.translate(table.concat(links, ";"))
        p.x('<AdditionalDependencies>%s;%%(AdditionalDependencies)</AdditionalDependencies>', links)
    end
end

p.override(p.vstudio.vc2010.elements, "lib", function(base, cfg, explicit)
    local calls = base(cfg, explicit)
    if cfg.kind == p.STATICLIB then
        table.insert(calls, myAdditionalStaticDependencies)
    end
    return calls
end)
